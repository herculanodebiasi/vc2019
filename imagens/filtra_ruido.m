img=imread('lena.png');
imshow(img);

% Criar ruído e adicionar na imagem original
sigma_ruido = 32;
ruido=randn(size(img)) * sigma_ruido;
img_ruido=img+ruido;
figure;imshow(img_ruido);

% Criar e aplicar o filtro gaussiano
sigma_filtro = 5;
tamanho_filtro = 31;

filtro_gauss = fspecial('gaussian', 
  tamanho_filtro, 
    sigma_filtro);
    
img_suavizada = imfilter(img_ruido, 
  filtro_gauss);
figure; imshow(img_suavizada);