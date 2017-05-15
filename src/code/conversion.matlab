for K = 1:41
	s1 = 's'
	s2 = num2str(K)
	dirname = strcat(strcat(s1,s2),'/')
	for I = 1:10
		s3 = num2str(I)
		s4 = '.pgm'
		filename = strcat(s3,s4)
		imagen = strcat(dirname,filename)
		A = imread(imagen)
		B = im2int16(A)
		output = strcat(imagen, '.dat')
		fid = fopen(output, 'w');
		fwrite(fid, B);
		fclose(fid);
	end
end