secret<-function(x,t){
L<-length(x)
val<-Inf
for(i in seq(1,L))
    if (x[i]>=t&x[i]<val){
        ind<-i
        val<-x[i]
    }
    c(val,ind)

}