// Helpers for inline assembly

.pseudocommand mov src:tar {
    lda src
    sta tar
}

//.pseudocommand loadFP src:dst1:dst2 {
		//ldx #3
	//loop:
		//lda src, x
		//sta dst1, x
		//.if (dst2.getType() != AT_NONE) sta dst2, x
		//dex
		//bpl loop
//}

//.pseudocommand mloadFP src1:dst1:src2:dst2:src3:dst3 {
		//ldx #3
	//loop:
		//lda src1, x
		//sta dst1, x
		//lda src2, x
		//sta dst2, x
		//.if (src3.getType() != AT_NONE) {
			//lda src3, x
			//sta dst3, x
		//}
		//dex
		//bpl loop
//}

//.pseudocommand argsFP arg1 : arg2 {
		//lda arg1
		//sta __fp_x1
		//loadFP arg1+1 : __fp_m1
		//.if (arg2.getType() != AT_NONE) {
			//lda arg2
			//sta __fp_x2
			//loadFP arg2+1 : __fp_m2
		//}
//}

//.pseudocommand returnFP ret {
		//lda __fp_x1
		//sta ret
		//loadFP __fp_m1 : ret+1
//}
