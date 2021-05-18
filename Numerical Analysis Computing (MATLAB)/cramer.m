[filename, pathname] = uigetfile ('*.*', 'Pick a file');
    file = [pathname, filename];%opening file
    fid = fopen(filename);
    M = fscanf(fid,'%g');
    n=M(1);%value n for the nxn matrix
    vector=M(2:1:end-n);%vector for matrix A
    %display(vector);
    A=reshape(vector,[],n);%turn vector into nxn matrix
    %display(A);
    A=A';%obtain the tranpose so A is in the correct format
    display(A);
    b=M(end-n+1:end)%vector for b
    display (b);
    res=zeros(n,1);%initialize the solution vector
    D=det(A);%determinant for A
    fprintf('determinant A = %i\n', D);
    if A==0
        error('There is no solution')
    elseif D==0
        error('There is no solution')
    end
    k=1;%j and k are arbritary elements to help with indices in order to get A1, A2, A3... etc.
    j=1;
    for i=1:n
        Aug=A;%initialize augmented matrix
        Aug(:,i)=b;%form augmented matrix
        deter=det(Aug);%obtain determinant for augmented matrix
        res(i)=(deter/D);%cramer's rule

        fprintf('determinant A%i = %i\n', j, deter);%print determinant for each Ai 

        j=j+1;


    end
    out=res;%output result

    for i=1:n
        fprintf('x%i = %i\n', k, res(i));%print each xi
        k=k+1;
    end
