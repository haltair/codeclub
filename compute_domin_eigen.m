function [e1,lambda1] = compute_domin_eigen(a)
  
  
  N = size(a,3);

  e1 = zeros(N,2);
  lambda1 = zeros(N,1);
  
  temp1 = a(1,1,:) + a(2,2,:);
  temp2 = a(1,1,:).*a(2,2,:);
  temp3 = a(1,2,:).*a(2,1,:);

  lambda1 = 0.5 * (temp1 + (temp1.^2 - 4*(temp2-temp3)).^(0.5));
  
  den = ((a(1,2,:).^2 + (lambda1-a(1,1,:)).^2).^(0.5));
  
  e1(:,1) = ((a(1,2,:)))./(den);
  e1(:,2) = ((lambda1-a(1,1,:)))./(den);