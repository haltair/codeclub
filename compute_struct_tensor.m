function I_struct_tensor = compute_struct_tensor(I_in,m,sigma)

  
  %red channel
  I = I_in(:,:,1);
  
  [LENGTH,WIDTH] = size(I);
  
  N = LENGTH*WIDTH;
    
  %Compute the gradient
  [grad_Iy,grad_Ix] = gradient(I);
  
  %Compute the tensor
  I_tensor_red = (repmat(reshape([grad_Ix(:)';grad_Iy(:)'],2,1,N),[1,2,1])).* ...
      (repmat(reshape([grad_Ix(:)';grad_Iy(:)'],1,2,N),[2,1,1]));
  
  
  %green channel
  I = I_in(:,:,2);
    
  %Compute the gradient
  [grad_Iy,grad_Ix] = gradient(I);
  
  %Compute the tensor
  I_tensor_green = (repmat(reshape([grad_Ix(:)';grad_Iy(:)'],2,1,N),[1,2,1])).* ...
      (repmat(reshape([grad_Ix(:)';grad_Iy(:)'],1,2,N),[2,1,1]));
  
  %blue channel
  I = I_in(:,:,3);
  
  %Compute the gradient
  [grad_Iy,grad_Ix] = gradient(I);
  
  %Compute the tensor
  I_tensor_blue = (repmat(reshape([grad_Ix(:)';grad_Iy(:)'],2,1,N),[1,2,1])).* ...
      (repmat(reshape([grad_Ix(:)';grad_Iy(:)'],1,2,N),[2,1,1]));
  
    
  I_tensor =  I_tensor_red+I_tensor_green+I_tensor_blue;
     
    
  %Build the Gaussian kernel
  h = fspecial('gaussian',m,sigma);
  
  I_struct_tensor = zeros(2,2,N);

  %Compute the structure tensor (smooth with the Gaussian channel wise)
  
  I_struct_tensor(1,1,:) = reshape(imfilter(reshape(I_tensor(1,1,:),LENGTH, ...
                                                    WIDTH),h,'replicate'),N,1);
  I_struct_tensor(1,2,:) = reshape(imfilter(reshape(I_tensor(1,2,:),LENGTH, ...
                                                    WIDTH),h,'replicate'),N,1);
  I_struct_tensor(2,1,:) = reshape(imfilter(reshape(I_tensor(2,1,:),LENGTH, ...
                                                    WIDTH),h,'replicate'),N,1);
  I_struct_tensor(2,2,:) = reshape(imfilter(reshape(I_tensor(2,2,:),LENGTH, ...
                                                    WIDTH),h,'replicate'),N,1);
    
  