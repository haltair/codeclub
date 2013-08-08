I = double(imread('sheehan_paint1.jpg'));

%some constants
m = 12; sigma = 0.5;
h_x = 1;h_y = 1;tau = 0.1;MAX_TIME = 5;

%Gaussian filter
h = fspecial('gaussian',m,2);

t = 0;

tic
while(t <= MAX_TIME)
  
  I_struct_tensor = compute_struct_tensor(I,m,5);
  [e1,lambda1] = compute_domin_eigen(I_struct_tensor);
      
  I_red   = imfilter(I(:,:,1),h,'replicate');
  I_green = imfilter(I(:,:,2),h,'replicate');
  I_blue  = imfilter(I(:,:,3),h,'replicate');
  I_hess_red   = mex_compute_imghessian(I_red,[size(I_red) h_x h_y]);
  I_hess_green = mex_compute_imghessian(I_green,[size(I_green) h_x h_y]);
  I_hess_blue  = mex_compute_imghessian(I_blue,[size(I_blue) h_x h_y]);
 
  I_hess = I_hess_red + I_hess_green + I_hess_blue;
  
  temp = (I_hess.*[e1(:,1).^2 2.*e1(:,1).*e1(:,2) e1(:,2).^2]);
   
  FLAG = -1*sign(sum(temp,2));
  temp_idx = find(isnan(FLAG) == 1);
  FLAG(temp_idx) = 0;
  
  I_red   = I(:,:,1);
  I_green = I(:,:,2);
  I_blue  = I(:,:,3);
  FIXED_PARAMS = [size(I_red) h_x h_y tau];
  I_red = mex_morphological_filter(I_red,FLAG,FIXED_PARAMS);
  I_green = mex_morphological_filter(I_green,FLAG,FIXED_PARAMS);
  I_blue = mex_morphological_filter(I_blue,FLAG,FIXED_PARAMS);

  I(:,:,1) = I_red;  I(:,:,2) = I_green;  I(:,:,3) = I_blue;
    
  t = t + tau;
  
end
toc    
imwrite(uint8(I),'shocked.jpg');
