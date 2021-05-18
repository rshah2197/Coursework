clear all;
[filename, pathname] = uigetfile ('*.*', 'Pick a file');%opening file
file = [pathname, filename];
fid = fopen(filename);
A = fscanf(fid,'%g');%scanning file
x = A(2:2:end-1);%vector of x components
y = A(3:2:end);%vector of y components
x0=A(end);%I am assuming that x0 appears last in the txt file.  Talked to TA about it.


n = length(x);%length of x and y should be the same
u = length(y);

	
	for k = n-1:-1:1%neville's formula
		y(1:k) = y(2:k+1) + ...
						 ( x0 - x(n-k+1:n) ) ./ ...
						 ( x(n-k+1:n) - x(1:k) ) .* ...
		         ( y(2:k+1) - y(1:k) );
	end
	p = y(1);%result
    
    fprintf('P(x_0) = %g\n', p);%print statement