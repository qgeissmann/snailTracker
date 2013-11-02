#!/bin/Rscript


findNonAmbiguous <- function(mat,max_speed = 0.01){
	single_blob <- ifelse(mat[,'nBlobs'] == 1,1 ,0)
	#~ 	In addition, to be unambiguous, the previous frame and the next 
	#~ 	frame have to be 1-blobed
	unambiguous <- ifelse(filter(single_blob,c(1,0,1)) == 2,TRUE,FALSE)
	ifelse(is.na(unambiguous),FALSE,unambiguous)
	# chop begining
	mat <- mat[unambiguous,]
	
	first <- which(unambiguous)[1]
	mat <- na.omit(mat[first:nrow(mat),])
	
	sdx <- diff(mat[,'X'])^2
	sdy <- diff(mat[,'Y'])^2
	dt <- diff(mat[,'t'])

	speed <- c(sqrt(sdx+sdy)/dt,NA)
	speed_wrong <- ifelse(speed > max_speed, 1, 0)
	#if any neighbour point has a wrind speed, the point has a wrong speed
	speed_wrong <- ifelse(filter(speed_wrong,c(1,1,1,1,1,1,1)) > 0, TRUE,FALSE)
	#speed anormally high = pblem
	
	mat <- na.omit(mat[!speed_wrong,])
	
	return (mat)
}
intrapolateXY <- function(mat){
	mat[,'t'] <- mat[,'t'] - mat[1,'t']
	dt <- diff(mat[c(1,nrow(mat)),'t'])
	x <- approx(mat[,"t"],mat[,"X"],n=dt+1)$y
	y <- approx(mat[,"t"],mat[,"Y"],n=dt+1)$y
	t <- approx(mat[,"t"],mat[,"Y"],n=dt+1)$x 
	mat <- cbind("t" = t,"X" = x, "Y" = y)
	return(mat)
}

smoothXY <- function(mat,k=1){
	mat[,'X'] <- runmed(mat[,'X'],k)
	mat[,'Y'] <- runmed(mat[,'Y'],k)
	return(mat)
}

plotXYT <- function(mat,title){
	ti <- sprintf("Input = '%s'\nFinal time = %ss",title,mat[nrow(mat),"t"])
	plot(mat[,"Y"] ~ mat[,"X"],xlim=c(0,1),ylim=c(0,1),col=terrain.colors(nrow(mat)),pch=20,xlab = "X", ylab = "Y",main = ti)
}
main <- function(argv){

	mat <- as.matrix(read.table(argv[1],sep=","))
	colnames(mat) <- c('t','X','Y','nBlobs','W','H')
	unambig_mat <- findNonAmbiguous(mat)
	smoothed_mat <- smoothXY(unambig_mat)
	homogeneous_mat <- intrapolateXY(smoothed_mat)
	pdf(sprintf("%s.pdf",argv))
	print(head(homogeneous_mat))
	cat("...\n")
	print(tail(homogeneous_mat))
	
	write.table(file = sprintf("filtered-%s",argv),x=homogeneous_mat,row.names=F,sep=",")
	plotXYT(homogeneous_mat, argv)
	dev.off()
	

}

argv <- commandArgs(TRUE)[1]
#~ argv <- "/tmp/test.csv"
mat <- main(argv);



