im_in = imread('albert-einstein.jpg');
kernel = [0 0 0; 0 0 1; 0 0 0];

im_out = im_in;
for i=1:10
   im_out = imfilter(im_out, kernel, 'same', 'conv');
end

subplot(1,2,1); imshow(im_in); title('Original');
subplot(1,2,2); imshow(im_out); title('Filtrada');