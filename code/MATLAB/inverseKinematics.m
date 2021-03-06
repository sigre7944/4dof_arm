% fileID = fopen('model.txt','wt');
%Cosine and Sine value
syms c1 c2 c3 c4;
syms s1 s2 s3 s4;

%d and a value
d1 = 50;
a1 = 65;
a2 = 185;
a3 = 115;
a4 = 80;

%Kinematics Model
t01 = [c1 -s1 0 0 ; s1 c1 0 d1; 0 0 1 0; 0 0 0 1 ];
t12 = [c2 -s2 0 a1 ; 0 0 1 0; -s2 -c2 0 0; 0 0 0 1 ];
t23 = [c3 -s3 0 a2 ; s3 c3 0 0; 0 0 1 0; 0 0 0 1 ];
t34 = [c4 -s4 0 a3 ; s4 c4 0 0; 0 0 1 0; 0 0 0 1 ];
t4e = [1 0 0 a4 ; 0 1 0 0; 0 0 1 0; 0 0 0 1 ];

%Final forward kinematics tranformation
t0e = t01*t12*t23*t34*t4e;

%Value in forward kinematics model
r11 = t0e(1,1);
r12 = t0e(1,2);
r13 = t0e(1,3);
px = t0e(1,4);
r21 = t0e(2,1);
r22 = t0e(2,2);
r23 = t0e(2,3);
py = t0e(2,4);
r31 = t0e(3,1);
r32 = t0e(3,2);
r33 = t0e(3,3);
pz = t0e(3,4);



% fprintf(fileID, '%s, %s, %s, %s \n' ,t0e(1,1), t0e(1,2), t0e(1,3), t0e(1,4));
% fprintf(fileID, '%s, %s, %s, %s \n' ,t0e(2,1), t0e(2,2), t0e(2,3), t0e(2,4));
% fprintf(fileID, '%s, %s, %s, %s \n' ,t0e(3,1), t0e(3,2), t0e(3,3), t0e(3,4));
% fprintf(fileID, '%s, %s, %s, %s \n' ,t0e(4,1), t0e(4,2), t0e(4,3), t0e(4,4));
% fclose(fileID);
