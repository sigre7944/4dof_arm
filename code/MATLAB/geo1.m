syms o1 o2 o3;
c1=cos(o1);
c2=cos(o2);
c3=cos(o3);
s1=sin(o1);
s2=sin(o2);
s3=sin(o3);
s12=sin(o1)*cos(o2)+cos(o1)*sin(o2);
c12=cos(o1)*cos(o2)-sin(o1)*sin(o2);
s123=s12*cos(o3)-c12*sin(o3);
c123=c12*cos(o3)+s12*sin(o3);
A = s1*s2*s123+c1*c2*c123;
syms C1 C2 C3 S1 S2 S3
B = C1*C3*S1*S2^2+C2*C3*S1^2*S2-C1*C2*S1*S2*S3+S1^2*S2^2*S3 +C1^2*C2^2*C3 - C1*C2*C3*S1*S2+C1^2*C2*S2*S3 + C1*C2^2*S1*S3;