###----EXAMPLE SIMPLE LINEAR REGRESSION--------
###--------------POLYMERS DATA-----------------
###############################################

# Consider the data set "polymers.csv".The response variable y is the amount of suspended 
# solids in a coal cleansing system. Using a quantitative regresor variable x, the pH of the
# cleansing tank is recorded. The cleansing operation involves the use of a polymer. In this
# case three different polymers were used in the experiment. We will fit a simple linear regression
# model to explore the association between the variables pH and amount of solids.
####

# 1) Read the data and look at scatters plot altogether and by groups
data <- read.csv("polymers.csv",header=TRUE)

pH <- data[,1]
solid <- data[,2]

# If you have problems loading the data set, run the lines below to enter the data
# pH <- c(6.5,6.9,7.8,8.4,8.8,9.2,6.7,6.9,7.5,7.9,8.7,9.2,6.5,7.0,7.2,7.6,8.7,9.2)
# solid <- c(292,329,352,378,392,410,198,227,277,297,364,375,167,225,247,268,288,342)

# 2) Scatterplot without distingushing groups
png(filename="scatterplot.png")

plot(pH,solid,type="p",pch=20, col="blue", main="Scatter Plot Solids vs pH", xlab="pH",ylab="Amount of Solids")
cor(pH,solid)  # Correlation between the two variables

dev.off()

# 3) Fit the simple linear regression model
slr <- lm(solid ~ pH) 
summary(slr)  # coefficients an t-tests
anova(slr)    # ANOVA table

# 4) Confidene intervals for the coefficients
confint(slr, level=0.95)

png(filename="slr.png")

# 5) Add the regression line in the scatter-plot
plot(pH,solid,type="p",pch=20, col="blue", main="Scatter Plot", xlab="pH",ylab="Amount of Solids")
abline(slr)  # Add the regression line in the scatterplot

dev.off()

# 6) Do calculations "by hand" using the values below in the formulas covered in class
Sx <- sd(pH)         # Sample standard deviation of X
Sy <- sd(solid)      # Sample standard deviation of Y
Xbar <- mean(pH)     # Sample mean of X
Ybar <- mean(solid)  # Sample mean of Y
r <- cor(pH,solid)   # Correlation between X and Y

# How to obtain z-scores and t-scores in R?
qt(0.025, 17, lower.tail=FALSE)
qnorm(0.025, mean=0, sd=1, lower.tail = FALSE)

# How to obtain P-values in R?
pt(1.64, 17, lower.tail=FALSE)
pnorm(1.64, mean=0, sd=1, lower.tail=FALSE)

###########################
