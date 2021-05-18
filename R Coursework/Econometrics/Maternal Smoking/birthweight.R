#Ryan Shah 
#Econ 322 Homework 1

data13<-read.csv('birthweight.csv', head=T, sep=",")
newdata13 <- data13[data13$smoke!="9",]

nonsmoke <- newdata13[newdata13$smoke!="1",]
nonsmoke_mean <-mean(nonsmoke$bwt) 

 

smoke <- newdata13[newdata13$smoke!="0",]
mean(smoke$bwt)
smoke_mean <- (mean(smoke$bwt))

nonsmokemother <- nonsmoke$bwt
smokermother <- smoke$bwt


t2 <- t.test(nonsmokemother,smokermother)
t3 <- t.test(nonsmokemother,smokermother, conf.level = .1)
t1 <- t.test(nonsmokemother,smokermother, conf.level = .01)

library(broom)
library(purrr)

t_tab <- map_df(list(t1, t2, t3), tidy)

t_names <- c("***p>.01", "**p>.05", "*p>.1")

t_tab$P_Value = t_names

write.table(t_tab, file = "pvaluetable1.csv",row.names=FALSE, na="",col.names=TRUE, sep=",")

p_table<-read.csv('pvaluetable1.csv', head=T, sep=",")


sd(nonsmoke$bwt) 
nonsmoke_SD <- sd(nonsmoke$bwt) 

smoke_SD <- sd(smoke$bwt)

stderror_nonsmoke <- (sd(nonsmoke$bwt))/sqrt(length(nonsmoke$bwt))

stderror_smoke <- (sd(smoke$bwt))/sqrt(length(smoke$bwt))

nonsmoke_CIUpper <- mean(nonsmoke$bwt) + 1.96*((sd(nonsmoke$bwt))/sqrt(length(nonsmoke$bwt)))
nonsmoke_CILower <- mean(nonsmoke$bwt) - 1.96*((sd(nonsmoke$bwt))/sqrt(length(nonsmoke$bwt)))

string_nonsmoke_CILower <- paste("[", as.character(nonsmoke_CILower))
string_nonsmoke_cIUpper <- paste(as.character(nonsmoke_CIUpper), "]")

nonsmoke_CI <- paste(string_nonsmoke_CILower, string_nonsmoke_cIUpper, sep=",")


smoke_CIUpper <- mean(smoke$bwt) + 1.96*stderror_smoke
smoke_cILower <- mean(smoke$bwt) - 1.96*stderror_smoke

string_smoke_CILower <- paste("[", as.character(smoke_cILower))
string_smokE_cIUpper <- paste(as.character(smoke_CIUpper), "]")

smoke_CI <- paste(string_smoke_CILower, string_smokE_cIUpper, sep=",")



smoke_CI_Bracket <- c()
means_difference <- mean(nonsmoke$bwt) - mean(smoke$bwt)

std_nonsmoke_difference <- sqrt(sum((nonsmoke-means_difference)^2/(length(nonsmoke$bwt)-1)))
std_smoke_difference <- sqrt(sum((smoke-means_difference)^2/(length(smoke$bwt)-1)))

print(means_difference)

write.csv(nonsmoke, "nonsmoke.csv")

print(length(nonsmoke$bwt))

write.csv(smoke, "smoke.csv")

difference_stderror_nonsmoke <- (means_difference)/sqrt(length(nonsmoke$bwt))

difference_stderror_smoke <- (means_difference)/sqrt(length(smoke$bwt))

pre_SDE_difference <-((nonsmoke_SD)^2/(length(nonsmoke$bwt))+((smoke_SD)^2)/(length(smoke$bwt)))

standard_error_difference <- sqrt(pre_SDE_difference)
as.numeric(unlist(standard_error_difference))

#new_nonsmoke_CIUpper <- means_difference + 1.96*standard_error_difference
#new_nonsmoke_CILower <- means_difference - 1.96*standard_error_difference

#new_smoke_CIUpper <- means_difference + 1.96*standard_error_difference
#new_smoke_cILower <- means_difference - 1.96*standard_error_difference

means_difference_CIUpper <- means_difference + 1.96*standard_error_difference
means_difference_CILower <- means_difference - 1.96*standard_error_difference

string_means_difference_CILower <- paste("[", as.character(means_difference_CILower))
string_meansdifference_cIUpper <- paste(as.character(means_difference_CIUpper), "]")

means_difference_CI <- paste(string_means_difference_CILower, string_meansdifference_cIUpper, sep=",")

length_nonsmoke <- length(nonsmoke$bwt)
length_smoke <- length(smoke$bwt)



#df_numerator <- ((nonsmoke_SD^2)/(length_nonsmoke)+(smoke_SD^2)/(length_smoke))^2
#df_denomenator <- ((1/(length_nonsmoke-1))*(nonsmoke_SD^2/length_nonsmoke))^2+((1/(length_smoke-1))*(smoke_SD^2/length_smoke))^2
#df <- (df_numerator)/(df_denomenator)



#pooled_variance <- ((nonsmoke_SD^2)*(length_nonsmoke-1)+(smoke_SD^2)*(length_smoke-1))/(length_nonsmoke+length_smoke-2)

#pooled_sd <- (pooled_variance^0.5)*((1/length_nonsmoke)+(1/length_smoke))^0.5

#t <- (means_difference)/(pooled_sd)

#p_value <- pvalue(t,accuracy =.05)
#p_value_99 <- pvalue(t, accuracy = .001, decimal.mark = ".", add_p = FALSE)

#z_score <- (means_difference)/(standard_error_difference)

#z_scoreA <- (mean(smoke$bwt) - mean(nonsmoke$bwt))/(standard_error_difference)

#pvalue_95 <- exp(???0.717*(z_score) ??? 0.416*(z_score)^2) 

#write.table(smoke_cILower, file = "MyData.csv",row.names="Smoker" "non smoker", na="",col.names=FALSE, sep=",")

#write.table(df, file = "MyData2.csv",row.names=TRUE, na="",col.names=TRUE, sep=",")

df <- data.frame(c(" ", "Non Smoker", "Smoker", "Difference"), Mean = c("Mean", nonsmoke_mean, smoke_mean, means_difference), Standard_Error = c("Standard Error", stderror_nonsmoke, stderror_smoke, standard_error_difference), confidence_interval_95 = c("Confidence Interval (95%)", nonsmoke_CI, smoke_CI, means_difference_CI))

write.table(df, file = "MyData3.csv",row.names=FALSE, na="",col.names=FALSE, sep=",")

mytable<-read.csv('MyData3.csv', head=T, sep=",")

ftb <- read.table('MyData3.csv', header = FALSE, sep = ",", quote = "\"'")

#library(expss)
#data('MyData3.csv')

#dataf = apply_labels(ftb, V2 = c("Mean"=1), V3 = c("StandardError" = 2), V4 = c("confidence_interval_95" = 3))



#tb <- table(df)
