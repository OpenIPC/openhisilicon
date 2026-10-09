// SPDX-License-Identifier: GPL-2.0
/*
 * npu_bench: runs a compiled .xmm graph through libxmedia_cl on whatever
 * libxmedia_npu is loaded, checks every run against golden outputs and
 * reports load time and per-inference latency. Used to cross-check this
 * library against the SDK's.
 *
 *   npu_bench <dir> [runs] [dump-dir]
 *
 * NPU_BENCH_INFO=1 prints every input and output tensor first (shape,
 * pitches, type, quantization), which is how a compiled model's contract is
 * read off on the camera. NPU_BENCH_ASYNC=1 times xmedia_cl_graph_submit()
 * plus xmedia_cl_wait_for_events() instead of the blocking
 * xmedia_cl_graph_process().
 *
 * "cpu" is the whole process's CPU time per run, so it includes the golden
 * comparison on uncached output buffers and libxmedia_cl's own queue threads;
 * "in process" is what accrues while the graph call itself runs. On about
 * three process starts in four, libxmedia_cl's idle queue thread spins (its
 * pthread_cond_timedwait deadline reads tv_nsec from its stack canary) and
 * inflates the first figure; see BENCHMARKS.md.
 *
 * <dir> holds neuron_network.xmm, input_data<N>.bin and output_data<N>.bin
 * as the SDK's sample/npu/xmm data does; with [dump-dir] the outputs of
 * the last run are written there as output_data<N>.bin.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

#include "xmedia_cl.h"

#define MAX_IO 8

int copy_file_to_buff(const char *file, void *buf, int off, size_t size);
int copy_buff_to_file(const void *buf, const char *file, int off, size_t size);

static double now_ms(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1e3 + ts.tv_nsec / 1e6;
}

static double cpu_ms(void)
{
	struct rusage ru;

	getrusage(RUSAGE_SELF, &ru);
	return (ru.ru_utime.tv_sec + ru.ru_stime.tv_sec) * 1e3 +
	       (ru.ru_utime.tv_usec + ru.ru_stime.tv_usec) / 1e3;
}

static int cmp_double(const void *a, const void *b)
{
	double x = *(const double *)a, y = *(const double *)b;

	return x < y ? -1 : x > y;
}

static unsigned int type_bytes(xmedia_cl_data_type type)
{
	switch (type) {
	case XMEDIA_CL_INT8:
	case XMEDIA_CL_UINT8:
		return 1;
	case XMEDIA_CL_INT16:
	case XMEDIA_CL_UINT16:
	case XMEDIA_CL_FP16:
		return 2;
	default:
		return 4;
	}
}

static unsigned int tensor_bytes(const xmedia_cl_tensor *t)
{
	return type_bytes(t->shape.type) * t->shape.dims[0] *
	       t->shape.dims[1] * t->shape.pch[1];
}

static int get_io(xmedia_cl_graph graph, int out, xmedia_cl_tensor_info_inout *io)
{
	int ret;

	memset(io, 0, sizeof(*io));
	ret = out ? xmedia_cl_graph_get_output(graph, 0, io) :
		    xmedia_cl_graph_get_input(graph, 0, io);
	if (ret || io->num > MAX_IO)
		return ret ? ret : -1;
	io->tensor = calloc(io->num, sizeof(xmedia_cl_tensor));
	return out ? xmedia_cl_graph_get_output(graph, io->num, io) :
		     xmedia_cl_graph_get_input(graph, io->num, io);
}

int main(int argc, char **argv)
{
	xmedia_cl_tensor_info_inout in, out;
	xmedia_cl_device_id *devices = NULL;
	xmedia_cl_context context = NULL;
	xmedia_cl_graph graph = NULL;
	xmedia_cl_u32 ndev = 0;
	xmedia_cl_s32 err = 0;
	const char *dir, *dump;
	void *golden[MAX_IO];
	double t0, load_ms, *lat, cpu0, cpu, c0, cpu_proc = 0;
	char path[256];
	int runs, i, n, bad = 0, ret;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <dir> [runs] [dump-dir]\n", argv[0]);
		return 2;
	}
	dir = argv[1];
	runs = argc > 2 ? atoi(argv[2]) : 100;
	dump = argc > 3 ? argv[3] : NULL;
	lat = calloc(runs, sizeof(*lat));

	t0 = now_ms();
	if ((ret = xmedia_cl_init()) ||
	    (ret = xmedia_cl_get_device_ids(XMEDIA_CL_DEVICE_ALL, NULL, &ndev)))
		goto fail;
	devices = calloc(ndev, sizeof(*devices));
	if ((ret = xmedia_cl_get_device_ids(XMEDIA_CL_DEVICE_ALL, devices, &ndev)))
		goto fail;
	context = xmedia_cl_create_context(ndev, devices, &err);
	if ((ret = err))
		goto fail;
	snprintf(path, sizeof(path), "%s/neuron_network.xmm", dir);
	if ((ret = xmedia_cl_graph_loadmodel_from_file(&context, path, &graph)))
		goto fail;
	if ((ret = get_io(graph, 0, &in)) || (ret = get_io(graph, 1, &out)))
		goto fail;
	load_ms = now_ms() - t0;
	if (getenv("NPU_BENCH_INFO")) {
		for (i = 0; i < (int)in.num + (int)out.num; i++) {
			xmedia_cl_tensor *t = i < (int)in.num ? &in.tensor[i] :
					      &out.tensor[i - in.num];

			printf("%s%d id %d dims %u %u %u %u pch %u %u %u %u type %d "
			       "scale %g zp %d bytes %u\n",
			       i < (int)in.num ? "in" : "out",
			       i < (int)in.num ? i : i - (int)in.num, t->tensor_id,
			       t->shape.dims[0], t->shape.dims[1], t->shape.dims[2],
			       t->shape.dims[3], t->shape.pch[0], t->shape.pch[1],
			       t->shape.pch[2], t->shape.pch[3], t->shape.type,
			       t->quant.scale, t->quant.zp, tensor_bytes(t));
		}
	}

	for (i = 0; i < (int)in.num; i++) {
		snprintf(path, sizeof(path), "%s/input_data%d.bin", dir, i);
		xmedia_cl_graph_set_input(graph, in.tensor[i].tensor_id,
					  in.tensor[i].addr);
		copy_file_to_buff(path, in.tensor[i].addr, 0,
				  tensor_bytes(&in.tensor[i]));
	}
	for (i = 0; i < (int)out.num; i++) {
		golden[i] = malloc(tensor_bytes(&out.tensor[i]));
		snprintf(path, sizeof(path), "%s/output_data%d.bin", dir, i);
		copy_file_to_buff(path, golden[i], 0, tensor_bytes(&out.tensor[i]));
	}

	/* one untimed run: first touch of the command stream and buffers */
	if ((ret = xmedia_cl_graph_process(graph)))
		goto fail;
	cpu0 = cpu_ms();
	for (n = 0; n < runs; n++) {
		for (i = 0; i < (int)out.num; i++)
			memset(out.tensor[i].addr, 0, tensor_bytes(&out.tensor[i]));
		c0 = cpu_ms();
		t0 = now_ms();
		if (getenv("NPU_BENCH_ASYNC")) {
			/* submit + wait_for_events instead of the blocking call */
			xmedia_cl_event ev = NULL;

			ret = xmedia_cl_graph_submit(graph, &ev);
			if (!ret)
				ret = xmedia_cl_wait_for_events(1, &ev);
			if (ev)
				xmedia_cl_release_event(ev);
		} else {
			ret = xmedia_cl_graph_process(graph);
		}
		lat[n] = now_ms() - t0;
		cpu_proc += cpu_ms() - c0;
		if (ret)
			goto fail;
		for (i = 0; i < (int)out.num; i++)
			if (memcmp(out.tensor[i].addr, golden[i],
				   tensor_bytes(&out.tensor[i])))
				bad++;
	}
	cpu = (cpu_ms() - cpu0) / runs;
	if (dump)
		for (i = 0; i < (int)out.num; i++) {
			snprintf(path, sizeof(path), "%s/output_data%d.bin", dump, i);
			remove(path);
			copy_buff_to_file(out.tensor[i].addr, path, 0,
					  tensor_bytes(&out.tensor[i]));
		}

	qsort(lat, runs, sizeof(*lat), cmp_double);
	{
		double sum = 0;

		for (n = 0; n < runs; n++)
			sum += lat[n];
		printf("load %.2f ms; %d runs: min %.3f p50 %.3f avg %.3f "
		       "p99 %.3f max %.3f ms; cpu %.1f ms/run (%.1f in process); "
		       "golden mismatches %d\n",
		       load_ms, runs, lat[0], lat[runs / 2], sum / runs,
		       lat[runs * 99 / 100], lat[runs - 1], cpu, cpu_proc / runs, bad);
	}
	xmedia_cl_graph_unload(graph);
	xmedia_cl_release_context(context);
	xmedia_cl_release_device_ids(devices, &ndev);
	xmedia_cl_uninit();
	return bad ? 1 : 0;

fail:
	fprintf(stderr, "npu_bench: failed, error %d\n", ret);
	return 1;
}
