#R source code
png(filename = "./part2.png")
data = rchisq(10000, 1)  #part 2

par(mfrow=c(2,2))

hist(data, main= paste("Population Data,
Population mean =",round(mean(data),2),"sd =",round(sd(data),2)), col="grey", xlim=c(min(data),max(data)))

nsim = 10000

xbar3<-rep(0,nsim)
xbar30<-rep(0,nsim)
xbar100<-rep(0,nsim)

for(i in 1:nsim){
	sp3 = sample(data,3)
	xbar3[i]<-mean(sp3)
	sp30 = sample(data,30)
	xbar30[i]<-mean(sp30)
	sp100 = sample(data,100)
	xbar100[i]<-mean(sp100)}

hist(xbar3, main= paste("Sample size = 3,
mean =", round(mean(xbar3),2),"sd =",round(sd(xbar3),2)), col= "red", xlim=c(min(data),max(data)))
hist(xbar30, main= paste("Sample size = 30, 
mean =", round(mean(xbar30),2),"sd =",round(sd(xbar30),2)), col= "orange", xlim=c(min(data),max(data)))
hist(xbar100, main= paste("Sample size = 100, 
mean =", round(mean(xbar100),2),"sd =",round(sd(xbar100),2)), col= "green", xlim=c(min(data),max(data)))

dev.off()