clear;

[filename, pathname] = uigetfile ('*.*', 'Pick a file');
   

    file = [pathname, filename];
    fid = fopen(filename);
    A = fscanf(fid,'%g');%open compatible file
    degree=A(1);%degree not useful in the way I did it
    a=A(2:1:end-1);%vector of the coefficients
    n=length(a);%length of that vector
    x0=A(end);%our starting value
    f=a(n);%initializing f and g
    g=a(n);

for i=n-1:-1:2
    f=f*x0+a(i);%first step of horners
 
    g=g*x0+f;%second step of horners
     
    h=f*x0+a(1);%final value for P'
 
 
end


fprintf('The value of P(%i) is equal to %i\n',x0,g);%print statements
fprintf("The value of P'(%i) is equal to %i\n",x0, h);



    


