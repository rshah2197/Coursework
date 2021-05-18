
[filename, pathname] = uigetfile ('*.*', 'Pick a file');
file = [pathname, filename];%open, scan a compatible file
fid = fopen(filename);
A = fscanf(fid,'%g');
x = A(3:2:end);%vector of x values
y = A(4:2:end);%vector of y values
n = A(2);%n is the degree
q = A(1);%q is the numver of points


      
        
for i=1:n+1%this creates the matrix with the sums of x to a certain power
    for j=1:n+1
        p=i+j-2;
        M(i,j)= sum(x.^p);
        M(1,1)=q;
    end
end
        
%display(M);
 
%b=[sum(y);sum(y.*x);sum(y.*(x.^2));sum(y.*(x.^3))];

    e=0;%e is a value that helps with the indexing in order to create the b vector
    for v=1:n+1
       
        %for e=1:n
        b(v,1)=sum(y.*(x.^e));
        e=e+1;
      %  b=[sum(y.*(x.^0); ...
     %    sum(y.*(x.^n))];

    
    end
    



a=inv(M)*b;%solve for the coefficient matrix

coefficients=a';
fprintf('The Coefficients are \n')


%create the xx values for plotting
lower=A(3);
upper=A(end-1);
xx=[lower:0.1:upper];

%q and w help with the indices to create yy
q=2;
w=1;
yy=a(1);
for q=2:n+1
    
	%create yy
    yy=yy+a(q)*xx.^(w)
    q=q+1;
    w=w+1;
    
       
end
        

%yy=a(1) + a(2)*xx + a(3)*xx.^2 + a(4)*xx.^3;

%plot
plot(xx,yy,x,y,'*');
display(coefficients);




