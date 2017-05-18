function [ output_args ] = convert( input_args )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
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
			B = im2uint8(A)
			output = strcat(imagen, '.dat')
            dlmwrite(output, B);
			%fid = fopen(output, 'w');
			%fwrite(fid, A);
			%fclose(fid);
        end
    end
end
