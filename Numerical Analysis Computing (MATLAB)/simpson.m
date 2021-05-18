clear;
format long;
[filename, pathname] = uigetfile ('*.*', 'Pick a file');%opening file
file = [pathname, filename];
fid = fopen(filename);
A = fscanf(fid,'%s');%scanning file

k=A(end);
%if k=='0';
l=A(end-1);
m=strcat(l,k);
M=str2num(m);


bb=A(end-2);
b=str2num(bb);
aa=A(end-3);
a=str2num(aa);
funct=A(1:1:end-4);
f=inline(funct);

h=(b-a)/(2*M);
s1=0;
s2=0;
for k=1:M
    x=a+h*(2*k-1);
    s1=s1+feval(f,x);
end
for k=1:(M-1)
    x=a+h*2*k;
    s2=s2+feval(f,x);
end
s=h*(feval(f,a)+feval(f,b)+4*s1+2*s2)/3;
%display(s);

fprintf('The approximate value of the integral with %i subintervals is %g\n', M,s);



