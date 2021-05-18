superset<-read.csv('NJHealth1000.csv', head=T, sep=",")

#superset2<-read.csv('NJHealth1000.csv', head=T, sep=",")


str(superset) #summarize dataset (look at this along with codebook)
summary(superset) # summary of all the variables
names(superset)

superset$agesq=superset$age*superset$age
sex<-superset$sex

#superset$agenum=as.numeric(superset$age)

superset$sexf=factor(sex)

race<-superset$race
superset$racef=factor(race)

educ<-superset$educ
superset$educf=factor(educ)

#subset1<-subset(superset2,id & sex & age & race & educ & married & veteran & children & veteran & employed & middleincome & highincome & weight & height & bmi & insured & smoker & genhlth & avgDrinksPerDay & exercise30)

#subset2 <- superset[superset$id, superset$sex, superset$age, superset$race, superset$educ, superset$married, superset$veteran, superset$children, superset$veteran, superset$employed, superset$middleincome, superset$highincome, superset$weight, superset$height, superset$bmi, superset$insured, superset$smoker, superset$genhlth, superset$avgDrinksPerDay, superset$exercise30]


age, gender, income, married, height
firstset = subset(superset, select = -c(id, state, lowincome, sex, race, educ, veteran, children, employed, bmi, insured, smoker, genhlth, avgDrinksPerDay, exercise30, racef, educf) )

reg1<-lm(weight ~ agesq+age+middleincome+highincome+married+height+sexf, data=firstset)
summary(reg1)$coef

install.packages("car")
install.packages("carData")
library(car)
library(carData)


install.packages("sandwich")
install.packages("lmtest")
install.packages("zoo")
library(zoo)
library(lmtest)
library(sandwich)

reg1_n <- coeftest(reg1, vcov=vcovHC(reg1))



#age, gender, income, married, height, race, education
secondset = subset(superset, select = -c(id, state, lowincome, sex, veteran, children, employed, bmi, insured, smoker, genhlth, avgDrinksPerDay, exercise30, race, educ) )

reg2<-lm(weight ~ agesq+sexf+middleincome+highincome+age+married+height+racef+educf, data=secondset)
summary(reg2)
reg2_n<-coeftest(reg2, vcov=vcovHC(reg2))

#highincome <- superset$highincome
#middleincome <- superset$middleincome

linearHypothesis(reg1, c("highincome=0", "middleincome=0"), white.adjust=T)
linearHypothesis(reg1, c("highincome", "middleincome", white.adjust=T)

linearHypothesis(reg1, c("highincome=0", "middleincome=1), white.adjust=T)

linearHypothesis(reg2, c("racef2=0","racef3=0","racef4=0","racef5=0"), white.adjust=T)
linearHypothesis(reg2, c("educf2=0","educf3=0","educf4=0","educf5=0", "educf6=0"))


#age, gender, income, married, race, education
thirdset = subset(superset, select = -c(id, state, height, lowincome, sex, veteran, children, employed, bmi, insured, smoker, genhlth, avgDrinksPerDay, exercise30, race, educ) )

reg3<-lm(weight ~ age+agesq+sexf+middleincome+highincome+married+racef+educf, data=thirdset)
summary(reg3)
reg3_n<-coeftest(reg3, vcov=vcovHC(reg3))

install.packages("sjPlot")
install.packages("sjmisc")
install.packages("sjlabelled")
install.packages("moderndive")
install.packages("tidyverse")
library(sjPlot)
library(sjmisc)
library(sjlabelled)

#reg1table <- tab_model(reg1)

totaltable <- tab_model(reg1, reg2, reg3)

#get_regression_table(reg1, digits = 6, print = FALSE)





