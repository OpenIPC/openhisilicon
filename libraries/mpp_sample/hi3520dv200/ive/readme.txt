1.  the sample in IVE_test_memory is check whether memory is modified by other modules exclude cpu or whether there is error when loading and saving data.It is useful for demo board test. It has several step.  
	ImageDst(i,j)=ImageOri(i,j);		// copy operation
	ImageHist(i,j)=ImageOri(i,j)-ImageDst(i,j); // sub operation
	Hist(i)=Histogram(ImageHist(i,j));			// get image's histogram
	if(Hist(0)!=image_width*image_height)		// if zero data count is not equal image size,then there's error happen within ive runing.
	{
		// error process
	}
 	
	s32Ret=HI_MPI_IVE_DMA(&IveHandleDMA,&stOri,&stDst,bInstant);					// copy data from const data array to destination memory area. bInstant is HI_FALSE,it allows add another node to current list for it is not our last operation.
	s32Ret=HI_MPI_IVE_SUB(&IveHandleSUB,&stOri,&stSubInfo,&stSub,eOutFmt,bInstant);			// compare const data array and destination memory area's data,if there are any difference,then sub result has none zero data. bInstant is HI_FALSE,it allows add another 																// node to current list for it is not our last operation.
	s32Ret=HI_MPI_IVE_HIST(&IveHandleHIST,&stHistInfo,&stHist,HI_TRUE);					// check sub result,if sub result has none zero data,then the first element in histogram array is not equal to image size. bInstant is HI_TRUE,this operation will be last 																// node in any  list.
	s32Ret=HI_MPI_IVE_Query(IveHandleHIST,&bFinished,1);							// check whether the job of histogram operation is finished. In this function call,bInstant is HI_TRUE,it will block until operation is 																// finished.
		
	In this sample,cache is not used, cache flush operation is not necessary.
	
	
	
	
2. the sample in ive_detect is a simple occlusion detection using mean and variance of image block.
	mean(i,j)= sum(Image(i*block_w,j*block_h),block_w,block_h);
	variance(i,j)= sum( (Image(i*block_w,j*block_h)-mean(i,j))*(Image(i*block_w,j*block_h)-mean(i,j)),block_w,block_h);
	if mean and variance meet the rules,then it generate alarm.   
    when blocks is large, it can use integral operations to accelerate calculation. 	
	s32Ret=HI_MPI_VI_GetFrameTimeOut(0,&stFrameInfo,0);							// get frame from vi, u32MilliSec is 0 means this is block operation. it will block until it get's vi frame. if use vpss frame,then it 																// need to sleep to wait  when it return the failure while get vpss frame.   
															// while(1)
															// {
 															//	s32Ret=HI_MPI_VPSS_UserGetFrame(0,0,&stFrameInfo);
															//     if(s32Ret!=HI_FAILURE)
															//     {
															//		break;
															//	 }
															//      usleep(40000);
															// }
	s32Ret=HI_MPI_IVE_INTEG(&hIveHandle, &stSrc, &stDst, HI_TRUE);					// get integral data of image.bInstant is HI_TRUE,this operation will be last  node in any  list.	
	s32Ret=HI_MPI_IVE_Query(hIveHandle,&bFinished,1);							// check whether the job of integral operation is finished. In this function call,bInstant is HI_TRUE,it will block until operation is 																// finished.	
	// process by cpu to check data
	s32Ret=HI_MPI_SYS_MmzFlushCache(stDst.u32PhyAddr , pu64VirData , stSrc.u32Height * stSrc.u32Width*8);  //flush data when cpu operation is finished.
