#!/bin/Rscript




findNonAmbiguous <- function(mat){
	single_blob <- ifelse(mat[,'nBlobs'] == 1,1 ,0)
	#~ 	In addition, to be unambiguous, the previous frame and the next 
	#~ 	frame have to be 1-blobed
	unambiguous <- ifelse(filter(single_blob,c(1,0,1)) == 2,TRUE,FALSE)
	ifelse(is.na(unambiguous),FALSE,unambiguous)
	# chop begining
	mat <- cbind(mat,unambiguous)
	first <- which(unambiguous)[1]
	return (mat[first:nrow(mat),])
}

main <- function(argv){

	mat <- as.matrix(read.table(argv[1],sep=","))
	print(argv[1])
	colnames(mat) <- c('t','X','Y','nBlobs','W','H')
	unambig_mat <- findNonAmbiguous(mat)
	

	
	
#~ 	resolveAmbig(unambig, mat)
	print(unambig_mat)
#~ 	plot(mat[,'X'] ~ mat[,'t'])
#~ 	lines(non_ambig_mat[,'Y'] ~ non_ambig_mat[,'t'])
	

}

argv <- commandArgs(TRUE)[1]
#~ argv = "/tmp/test.csv"
main(argv);




