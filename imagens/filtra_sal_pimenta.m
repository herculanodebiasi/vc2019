img = imread('moon.jpg');
figure; imshow(img);

ruido = imnoise(img, 'salt & pepper', .1);
figure; imshow(ruido);

img_filtrada = medfilt2(ruido);
figure; imshow(img_filtrada);