clear all;

[filename, pathname] = uigetfile ('*.*', 'Pick a file');
file = [pathname, filename];
fid = fopen(filename);
A = fscanf(fid,'%g');
x = A(2:2:end);
y = A(3:2:end);
executespline(x,y,0);
