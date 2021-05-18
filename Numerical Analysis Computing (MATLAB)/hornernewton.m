clear;

[filename, pathname] = uigetfile ('*.*', 'Pick a file');
   

    file = [pathname, filename];%open a txt file with the format from the intructions
    fid = fopen(filename);
    A = fscanf(fid,'%g');
    degree=A(1);%degree not useful in this case but here it is
    a=A(2:1:end-1);%vector of the coefficients
    n=length(a);%length of the polynomial
    x0=A(end);%starting value
    f=a(n);%initializing f and g
    g=a(n);

for i=n-1:-1:2
    f=f*x0+a(i); %first part of horner's method
 
    g=g*x0+f; %second part of horner's method
     
    h=f*x0+a(1); %final derivative of polynomial
    
   
 
 
end

fprintf('The value of P(%i) is equal to %i\n',x0,g);%print statements
fprintf("The value of P'(%i) is equal to %i\n",x0, h);



 x0=x0-h/g;%one root of newtons. P(x0) and P'(x0) are used from horners

fprintf('One root of the polynomial is: %i\n', x0);%result
