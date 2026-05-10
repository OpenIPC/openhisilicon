# IVE ABI Notes — mined from the LiteOS-flavor SDK

Research notes assembled from the `Hi3516EV200_SDK_V5.0.1.2/` vendor SDK
(C05/LiteOS variant of the same V4 silicon supported on Linux). The goal
was to extract API/ABI facts that could harden or extend the existing
clean-room replacement at `kernel/ive_neo/` and `libraries/ive_neo/`.

## Headline findings

1. The userspace MPI surface in `libive_neo.a` is already a **complete
   superset** of the vendor LiteOS `libive.a`. ive_neo exports 51
   `HI_MPI_IVE_*` entry points; the vendor blob exports 49. The two extras
   ive_neo carries (`HI_MPI_IVE_Hog`, `HI_MPI_IVE_PerspTrans`) are declared
   in the vendor public header but *not* implemented in this LiteOS blob.
2. The vendor LiteOS kernel-side blob (`libhi3516ev200_ive.a`) and the
   Linux blob (`kernel/ive/ive.o`) export an **identical 114-symbol set**.
   They are the same source compiled twice — Linux glue swapped for LiteOS
   glue via the OSAL. This makes the blobs cross-checkable and confirms
   that the IVE register/algorithm code is OS-independent.
3. The kernel-side `ive_neo` driver (`kernel/ive_neo/ive_neo.c`,
   ~1,950 lines) is a clean-room rewrite, not a blob-symbol replacement:
   it provides the public `ive_std_mod_init/exit` + `g_ive_*` globals + a
   miscdevice with custom ioctls, and uses only ~15 of the vendor's
   internal 114 symbols. Symbol-level "gaps" here are not real gaps — the
   driver implements the same hardware functionality with a different
   internal architecture.
4. **No DWARF / debug info** survives in any of the blobs. Tactic 4
   (debug-info mining) yielded zero. All ABI evidence comes from symbol
   tables, format-string fragments, and section-content extraction.
5. Hardware constants extracted from `.data`:
   - **IVE IRQ number = 83 (decimal)** for hi3516ev200/gk7205v200
   - Default max IVE task-queue length = 512 nodes
   - Default XNN model max = 16, XNN info-node max = 16
   - Power-save enabled by default
6. Vendor toolchain string (informational): `HC&C V1R3C00SPC200B005_20190606
   GCC 6.3.0`, target Cortex-A7. The blob's `__FILE__` strings include
   `gk7205v200_ive.mod.c` — confirms the Linux blob is the goke-nolog
   build (consistent with commits `f54bea2` etc.).

## SDK naming context

`Hi3516EV200_SDK_V5.0.1.2` is **not** a hardware-generation bump from V4.
In HiSilicon's `Hi35xxVxxxRxxxCxxSPCxxy` convention, the `Cxx` segment is
the OS/compiler environment; this is the `C05` (Huawei LiteOS) variant of
the same V4 `hi3516ev200` silicon. The `Vxxx` (`V200`) is the silicon
revision. So the blobs in this SDK and the blobs in `kernel/ive/ive.o` /
`libive.so` from C01/C02 Linux SDKs are alternate OS builds of identical
algorithm code.

## Vendor IVE software-stack architecture

Recovered from archive-member structure and undefined-reference graph:

```
            userspace
            ┌─────────────────────────────────────┐
            │ libive.a (49 HI_MPI_IVE_* entries)  │
            │   mpi_ive.o          ─ MPI wrappers │
            │   check_param_user.o ─ user-side    │
            │                        validation   │
            │   inner_comm_user.o  ─ ioctl bridge │
            │                        (calls       │
            │                         ioctl(),    │
            │                         HI_MPI_SYS  │
            │                         _Mmz*)      │
            └────────────┬────────────────────────┘
                         │  ioctl
            ┌────────────▼────────────────────────┐
            │ libhi3516ev200_ive.a (kernel blob)  │
            │                                     │
            │   ive.o          ─ Dispatch         │
            │                    (IveProc,        │
            │                     md_proc,        │
            │                     ivp_proc,       │
            │                     svp_alg_proc)   │
            │                  ─ XNN engine       │
            │                    (ive_xnn_*)      │
            │                                     │
            │   check_param.o  ─ kernel-side      │
            │                    validation       │
            │                    (ive_check_*_    │
            │                     param)          │
            │                                     │
            │   fill_task.o    ─ Command-buffer   │
            │                    construction     │
            │                    (ive_fill_*_     │
            │                     task)           │
            │                                     │
            │   ive_drv.o      ─ Register I/O     │
            │                    (drv_ive_*,      │
            │                     g_ive_regs)     │
            │                                     │
            │   ive_init.o     ─ Module init      │
            │                                     │
            │   const_tables.o ─ (empty in this   │
            │                     build — feature │
            │                     disabled)       │
            └─────────────────────────────────────┘
                         │  MMIO
            ┌────────────▼────────────────────────┐
            │ IVE hardware (IRQ 83, mmz-mapped)   │
            └─────────────────────────────────────┘
```

The **two-tier param-check pattern** is notable: every op has a
user-side `ive_check_*_param_user` (cheap pre-flight) and a kernel-side
`ive_check_*_param` (security/trust). The user-side check rejects bad
calls before the ioctl; the kernel-side check defends against userspace
tampering. ive_neo's `libraries/ive_neo/src/ive_internal.h` already
implements user-side image validation in the same shape.

## Userspace MPI: full implementation map

49 entry points in the vendor `libive.a`, all 49 implemented in
`libive_neo.a`:

| Category | Vendor | ive_neo |
|---|---|---|
| Memory ops | DMA, Map, Resize, CSC, FilterAndCSC, 16BitTo8Bit | ✓ |
| Filtering | Filter, OrdStatFilter, Sobel, MagAndAng, NormGrad | ✓ |
| Morphology | Dilate, Erode | ✓ |
| Logical | And, Or, Xor | ✓ |
| Arithmetic | Add, Sub, SAD | ✓ |
| Threshold | Thresh, Thresh_S16, Thresh_U16 | ✓ |
| Histogram | Hist, EqualizeHist, Integ | ✓ |
| Edge | CannyHysEdge, CannyEdge | ✓ |
| Binary blobs | CCL | ✓ |
| Texture | LBP, NCC | ✓ |
| Tracking | LKOpticalFlowPyr, STCandiCorner, STCorner | ✓ |
| Bg/Fg | GMM, GMM2, GradFg, MatchBgModel, UpdateBgModel | ✓ |
| ML inference | ANN_MLP_{Load,Predict,Unload}, SVM_{Load,Predict,Unload}, CNN_{Load,Predict,Unload,GetResult} | ✓ |
| Lifecycle | Query | ✓ |
| **Extras only in ive_neo** | — | Hog, PerspTrans |

Entry points the vendor public header declares but **neither** ive_neo nor
the LiteOS blob implements:
- `HI_MPI_IVE_KCF_*` (11 functions: CreateCosWin, CreateGaussPeak,
  CreateObjList, DestroyObjList, GetMemSize, GetObjBbox, GetTrainObj,
  JudgeObjBboxTrackState, ObjUpdate, Process)
- `HI_MPI_IVE_Hog`, `HI_MPI_IVE_PerspTrans` (vendor declares, doesn't ship;
  ive_neo declares and ships)
- `HI_MPI_IVE_FILTER_AND_CSC` (declared shape — actual symbol is camel-case
  `HI_MPI_IVE_FilterAndCSC`)

KCF (Kernel Correlation Filter) tracking is therefore a vendor *promised
but not delivered* feature. If KCF support becomes a goal, it would be
greenfield work — there's no vendor implementation to reverse-engineer.

## Kernel-side internal API (recovered)

114 symbols in the vendor blob, grouped functionally:

### Register I/O primitives (`drv_ive_*`, in `ive_drv.o`)
- `drv_ive_init`, `drv_ive_exit`
- `drv_ive_set_base_addr` — set IVE register block base
- `drv_ive_read_regs`, `drv_ive_write_regs`, `drv_ive_write_timeout_regs`
- `drv_ive_en_int`, `drv_ive_clear_int`, `drv_ive_get_int_status`
- `drv_ive_is_list_int`, `drv_ive_is_timeout_int`
- `drv_ive_get_task_id`, `drv_ive_get_utili`
- `drv_ive_en_clk_gt` — clock gate enable
- `drv_ive_set_mem_speed`, `drv_ive_set_outstanding`
- `drv_ive_pre_func_filter`

### Per-op command-buffer builders (`ive_fill_*_task`, in `fill_task.o`)
One per HW op:
`ive_fill_{dma,filter,sobel,dilate,erode,thresh,thresh_s16,thresh_u16,
16bit_to_8bit,and,or,xor,add,sub,mag_and_ang,norm_grad,hist,integ,ccl,
canny_hys_edge,ord_stat_filter,sad,lk_optical_flow_pyr,candidate_corner,
st_box_flt_and_eigen,gmm2,map,xnn_forward}_task`

Plus helpers: `ive_calc_stride`, `ive_convert_phy_addr`.

Notably **absent** from this list: no `ive_fill_kcf_*`, no
`ive_fill_hog_*`, no `ive_fill_persp_trans_*` — these are not HW ops.
KCF/Hog/PerspTrans (where present) are pure userspace orchestration.

### Per-op kernel-side validators (`ive_check_*_param`, in `check_param.o`)
One per HW op, mirroring the `ive_fill_*_task` set.

### XNN engine (in `ive.o`)
The IVE block contains a small custom NN inference engine:
- Public: `ive_xnn_init`, `ive_xnn_exit`, `ive_xnn_forward`,
  `ive_xnn_forward_data_slice`, `ive_xnn_preproc`, `ive_xnn_unloadmodel`
- Recovered internal names (from `.rodata` strings, not exported as
  symbols — likely inlined or static): `ive_xnn_loadmodel`,
  `ive_xnn_parse_model`, `ive_xnn_loop_parse_every_layer`,
  `ive_xnn_check_layer_id`, `ive_xnn_check_model_header`,
  `ive_xnn_check_model_tmp_buf`, `ive_xnn_check_task_buf`,
  `ive_xnn_get_model_to_ref_inc`, `ive_xnn_handle_pic`,
  `ive_xnn_handle_roi`, `ive_xnn_handle_slice`, `ive_xnn_slice_process`,
  `ive_xnn_begin_vgs_job`, `ive_xnn_begin_vb_vgs_task`,
  `ive_xnn_fill_forward_node_info`, `ive_xnn_create_node_info`,
  `ive_xnn_pre_dst_data`, `ive_xnn_malloc_task`,
  `ive_xnn_mmz_malloc_cached`, `ive_xnn_mmz_malloc_no_cache`,
  `ive_xnn_default_case`
- Layer types (from per-case dispatch names):
  `conv`, `flatten`, `fc`, `eltwise`, `unpack`, `preproc`, `dma`
- Layer-specific helpers: `ive_xnn_parse_conv`, `ive_xnn_parse_flatten`,
  `ive_xnn_parse_fc`, `ive_xnn_parse_unpack`, `ive_xnn_parse_preproc`,
  `ive_xnn_parse_dma`, `ive_xnn_parse_eltwise`, plus
  `ive_xnn_layer_*_case` companions

The existing `kernel/ive_neo/ive_neo.c` already has XNN support (refs to
`xnn_model_ctx`, `ive_xnn_loadmodel`, `ive_xnn_unloadmodel`,
`ive_xnn_fwd_slice`, `ive_xnn_parse_conv_constprop_*`,
`ive_xnn_query`) — symbol-level evidence of substantive coverage.

### Top-level dispatch (in `ive.o`)
- `IveProc` — main IVE op dispatcher
- `md_proc` — motion-detect dispatcher
- `ivp_proc` — image-vector-preprocessing dispatcher
- `svp_alg_proc` — SVP (smart vision processor) dispatcher
- `ive_init`, `ive_umap_module_init`, `ive_umap_module_exit`
- `ive_check_mod_param`, `ive_wait_condition_callback`

### Module-level globals (in `ive.o` `.data`)
- `g_ive_drv_ops` — driver op table (80 bytes, populated by relocations)
- `g_ive_irq` = **83** (the IVE hardware IRQ on this silicon)
- `g_ive_node_num` = 512 (default)
- `g_ive_xnn_info_node_num` = 16
- `g_ive_xnn_model_num` = 16
- `g_ive_power_save_en` = 1
- `g_ive_mmz_base_addr` (uninitialized — set at runtime from MMZ)
- `g_ive_tab_addr` (uninitialized — runtime table base)
- `vendor_ive_driver` — 92-byte driver descriptor; first ~16 bytes hold
  pad/zeros, then `"ive\0"` (device name), then `0x1d = 29` (likely
  module/version magic), then mostly zeros with a couple of constants:
  `0x9140` and `0x1124` at fixed offsets — purpose unconfirmed.

## Struct field shapes (from format strings)

The `IVE_IMAGE_S` family (recovered from format-string fragments):
- `au64VirAddr[3]` — 3-plane virtual addresses (HI_U64)
- `au64PhyAddr[3]` — 3-plane physical addresses (HI_U64)
- `au32Stride[3]` — 3-plane strides (HI_U32)
- `u32Width`, `u32Height`
- (older API references `pu8PhyAddr[1]`, `pu8PhyAddr[2]` — probably
  legacy field name; modern is `au64PhyAddr[]`)

Per-op control struct field accesses leaking from format strings:
- `ctrl->stMem.u64PhyAddr`, `ctrl->stMem.u64VirAddr` (control struct
  embeds an `IVE_MEM_INFO_S stMem`)
- `stack->u64VirAddr`, `stack->u64PhyAddr` (Canny stack memory)
- `sad->au64VirAddr[0]`, `thr->au64VirAddr[0]` (SAD output buffers)
- `image->au64PhyAddr[i]`, `roi_arr[i].u64PhyAddr`, `blob_arr[i].u64PhyAddr`
  (array-of-images / array-of-blobs lifetimes)

These shapes match `libraries/ive_neo/include/hi_comm_ive.h` already, so
this is consistent rather than newly-discovered.

## Validation constraints (from format strings)

- `phy_addr(0x%llx) must be %d byte align!` — physical addresses must be
  N-byte aligned per op (alignment varies)
- `phy_addr(0x%llx) must be in [0x%llx ,0x%llx]!` — must be in mmz range
- `image width(%d) must be a multiply of 2!`
- `image height(%d) must be a multiply of 2!`
- `stride(%u) must be [%u,%u] and %d align`
- `stride(%d) must be greater than or equal to width(%d)`
- `image addr num(%d) must be less than or equal to %d` — implicit cap
  on number of image planes/addresses
- `the node info queue is full! u32EndIdx(%d),u16CurId(%d),
  g_ive_xnn_info_node_num(%d)` — XNN node-queue is bounded by
  `g_ive_xnn_info_node_num`
- `the waiting queue is full of task! end_idx = %d,g_ive_node_num = %d` —
  IVE task queue is bounded by `g_ive_node_num`

ive_neo enforces these in `ive_internal.h` and `ive_ops.c`.

## Sample-app exercise distribution (LiteOS SDK, 9 samples)

Coverage of the 49 vendor APIs by the bundled samples:

| Sample | Unique IVE APIs called | Notable APIs |
|---|---|---|
| `sample_ive_st_and_lk.c` | 6 | LKOpticalFlowPyr, STCandiCorner, STCorner |
| `sample_ive_gmm2.c` | 6 | GMM2 + Sobel/Filter pipeline |
| `sample_ive_canny.c` | 5 | CannyHysEdge, CannyEdge |
| `sample_ive_test_memory.c` | 4 | DMA, Sub variants |
| `sample_ive_od.c` | 3 | object-detection chain |
| `sample_ive_sobel_with_cached_mem.c` | 2 | Sobel + Query |
| `sample_ive_md.c` | 0 (uses `libmd.a`) | — out of IVE scope — |
| `sample_ive_qr.c` | 0 (uses `libqr.a` + quirc) | — out of IVE scope — |

Top-called APIs across all samples: `Query` (39 calls — the polling
primitive), `DMA` (8), `Sub` (6), `MagAndAng` (6), `Sobel` (4). Only ~17
of the 49 APIs are exercised by samples; coverage of the long tail
(NCC, Resize, ML inference, GMM, MatchBgModel/UpdateBgModel/GradFg, the
threshold variants, OrdStatFilter, EqualizeHist, FilterAndCSC, CSC, LBP,
Map, And/Or/Xor/Add) is **left to the developer**.

The `md` and `qr` samples link against `libmd.a` (`ivs_md.o`) and
`libqr.a` (`qr_alg.o`, `quirc.o`) respectively. `quirc` is a known
open-source QR library, suggesting the vendor `libqr.a` may be largely
the open quirc code wrapped — out of scope here, but worth flagging.

## Audit: XNN execution dispatch (verified)

ive_neo's `ive_build_task_nodes` (`kernel/ive_neo/ive_neo.c:198`)
dispatches on layer type via `switch (type)` after calling
`ive_layer_desc_size(type)` which knows all 7 descriptor sizes
(`{80, 48, 64, 96, 64, 48, 32}` for types 0–6). Per-type execution status:

| Type | Vendor name | ive_neo status | Evidence |
|---|---|---|---|
| 0 | Conv | **✓ Full impl** with tiling logic | `ive_neo.c:281–347` ~70 lines, IDA-derived field map for desc[3]…desc[78] |
| 1 | Flatten | **✓ Full impl** | `ive_neo.c:222–242` |
| 2 | FC | **⚠ Partial — TWO TEMP hardcoded values** | `ive_neo.c:243–272`: `node+32 = 0x400` and `node+44 = 0x20` are hardcoded "from vendor capture", per inline comment valid only for non-tiled FC. Tiled FC will produce wrong output. |
| 3 | Eltwise | **✗ No-op** | `ive_neo.c:351`: `case 3: case 6: break;` — descriptor is parsed for size but no HW node emitted |
| 4 | Unpack | **✓ Full impl** | `ive_neo.c:273–305` |
| 5 | Preproc | **✓ Correctly skipped** (CPU-side, no HW node) | `ive_neo.c:219`: `case 5: ... break;` |
| 6 | DMA-layer | **✗ No-op** | shares `case 3: case 6: break;` |

The empty `case 3: case 6:` handler means models using XNN Eltwise layers
(e.g., residual-connection ResNets) or XNN-DMA layers will silently produce
incorrect results — descriptors get advanced past correctly, but no HW
nodes are scheduled for those layers.

**Note:** "XNN-DMA layer" (type 6) is distinct from the standalone IVE DMA
op (`HI_MPI_IVE_DMA`); the latter is fully implemented in `ive_op_dma()`.

## Audit: input validation vs vendor constraints (verified)

Vendor kernel-side blob carries **397 unique constraint strings** (alignment
checks, stride bounds, dimension multiples-of-2, mode-enum range checks,
struct-field cross-checks like `s16HighThr ≥ s16LowThr`).

ive_neo validation surface is **substantially smaller**:

**Userspace (`libraries/ive_neo/src/ive_ops.c`):**
- NULL pointer checks via `IVE_CHECK_NULL` on caller args
- Generic helpers (`ive_op_2img`, `ive_op_3img`, `ive_op_4img`,
  `ive_op_2src`) cap `ctrl_size` to prevent buffer overruns in the packed
  ioctl arg
- No alignment, stride, dimension, or mode-value checks

**Kernel-side (`kernel/ive_neo/ive_neo.c`, op handlers `ive_op_*`):**
- No NULL checks on user-passed phys/virt addresses
- No alignment checks on `phy_addr`
- No range checks on width/height/stride
- No mode-enum validation
- Counter-example (`ive_op_dma`, line 994): unpacks buf into src/dst/ctrl
  pointers, builds 208-byte HW node, submits — zero defensive checks

This is a **significant divergence from the vendor's defense-in-depth
posture**, but functionally the consequences are bounded:
- Kernel-side trusts userspace — a malicious caller can pass arbitrary
  phys addresses to MMIO. The IVE engine reads/writes whatever phys
  range it's told, including arbitrary RAM. This is a real local-DoS
  surface (and on systems without IOMMU, potentially data corruption).
- Invalid alignments/dimensions usually produce a HW timeout or garbage
  output rather than a kernel oops — *probably*. Not characterized.

The format-string text in the vendor blob preserves the exact constraint
spec for each op. Translating the full surface into ive_neo would be
mechanical but voluminous — ~397 distinct checks. A *defensive-minimum*
slice (verify `phy_addr` lies inside MMZ, verify width × stride bytes
fit, verify mode enums are in declared range) would close the worst
gaps without trying to mirror every vendor sanity check.

**Per-op constraint examples** captured from vendor blob format strings
(non-exhaustive):

- DMA: `phy_addr` must be N-byte aligned (varies), src/dst dims must be
  multiples of 2, stride ≥ width, stride must be aligned
- Sobel/MagAndAng: same alignment plus
  `ctrl->u8MaskSize must be in [...]`, output buffers cannot overlap input
- CCL: `ctrl->u16InitAreaThr` and `ctrl->u8Step` constraints,
  `dst->u32Stride must be sufficient for label storage`
- Thresh family: `ctrl->u8MinVal ≤ ctrl->u8MaxVal`,
  `s16HighThr ≥ s16LowThr`, `enMode` in valid enum range
- LK Optical Flow: `u16PtsNum in [1, max]`, pyr arrays sized to
  `pyr_level`, `u0q8MinEigThr can't be 0`, `u0q8Eps can't be 0`,
  `u0q8QualityLevel can't be 0`
- ML inference (CNN/SVM/ANN_MLP): `ctrl->src_num` must equal
  `model_info->src_num` (× scl_num for some), strict input-shape match
  to model

## Recommended next steps for `ive_neo`

Priority-ordered:

1. **Cross-validate ive_neo against vendor blob behavior** (highest
   value). Pick 3–5 representative ops (DMA, Sobel, Filter, CCL, GMM2) and
   run identical inputs through both `kernel/ive/ive.o + libive.so` and
   `kernel/ive_neo/ + libive_neo.so` on the hi3516ev300 board
   (10.216.128.68). Compare output buffers byte-for-byte. Any deltas are
   real bugs in ive_neo.

2. **Adopt vendor input-validation constraints**. The format-string
   constraints (alignment requirements per op, stride min/max, etc.) are
   per-op specific and tighter than what `ive_internal.h::ive_check_image`
   currently enforces. Audit each `HI_MPI_IVE_*` impl in `ive_ops.c`
   against the corresponding `Error(%#x): check X parameters failed!`
   constraint set extracted from the blob.

3. **Confirm `g_ive_irq = 83` matches the device tree** for hi3516ev200
   and gk7205v200. If `kernel/ive_neo/ive_neo.c` reads IRQ from DT/probe,
   no-op; if hard-coded, verify the constant.

4. **Audit XNN coverage**. The vendor's XNN engine has 7 layer types
   (`conv`, `flatten`, `fc`, `eltwise`, `unpack`, `preproc`, `dma`). Walk
   `kernel/ive_neo/ive_neo.c` and confirm each is parsed/dispatched. The
   recovered internal name list (above) is a checklist.

5. **Skip Hog / PerspTrans verification**. ive_neo declares and exports
   them but the vendor LiteOS blob doesn't implement either; we can't
   cross-check behaviorally. Leave the ive_neo implementations as-is or
   document them as untested.

6. **Defer KCF**. Neither side ships an implementation. KCF would be
   greenfield development, not blob replacement. Out of scope until
   demanded.

7. **Use the LiteOS samples as integration tests**. Port
   `sample_ive_canny.c`, `sample_ive_gmm2.c`, `sample_ive_st_and_lk.c`
   into `kernel/ive_neo/test/qemu/` (or a userspace equivalent under
   `libraries/ive_neo/test/`). They're vendor-blessed reference flows
   and exercise the most algorithmically complex ops.

## Out of scope

- Replacing the IVE userspace blob: ive_neo already does this completely.
- Replacing any non-IVE module.
- Changes targeting CV500 or any non-V4 silicon.
- Building openhisilicon for the LiteOS target itself.

## Sources

- LiteOS SDK: `/tmp/hisi-v5/sdk/Hi3516EV200_SDK_V5.0.1.2/`
  - userspace blob: `package/mpp/lib/libive.{a,so}` (3 archive members)
  - kernel-side blob: `package/mpp/lib/libhi3516ev200_ive.a` (6 members)
  - public headers: `package/mpp/include/{hi_ive.h,hi_comm_ive.h,mpi_ive.h}`
  - sample apps: `package/mpp/sample/ive/`
- Linux blob: `kernel/ive/{ive.o, ive_nolog.o}` (single linked TU,
  goke-nolog flavor per `__FILE__` strings naming `gk7205v200_ive.mod.c`)
- ive_neo userspace: `libraries/ive_neo/{src/, include/, libive_neo.{a,so}}`
- ive_neo kernel: `kernel/ive_neo/{ive_neo.c (~1,950 lines), ive_init.c}`
- Tooling: host `binutils` (nm, objdump, readelf, strings); the
  cross-toolchain reported in the LiteOS blob's `.comment` is
  `HC&C V1R3C00SPC200B005_20190606 GCC 6.3.0`.
