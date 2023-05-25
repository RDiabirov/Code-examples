

void angle_to_hydrocylinder_unlinear_correction(float *x1, float *a, float *b, float *psi, int i, float alpha)
{
x1[0] = (x1[0]*a[i]*0.000001*b[i]*sin(Kg_r*(psi[i]+alpha)))/(sqrt(a[i]*a[i]*0.000001+b[i]*b[i]*0.000001-2*a[i]*b[i]*0.000001*cos(Kg_r*(psi[i]+alpha))));
}


void dead_zone(float *x1,float *x2,float a,float b)
{
    ///
    if (a<b)
    {
        if (x1[0]<a) x2[0]=x1[0]-a;
        else if (x1[0]>b) x2[0] = x1[0] - b;
        else x2[0]=(double)0.0;
    } //end if a<b
    else
    {
        if (x1[0]>a) x2[0]=x1[0]-a;
        else if (x1[0]<b) x2[0] = x1[0] - b;
        else x2[0]=(double)0.0;
    } //end if a>b
}    




void n_zone_jj(float *x1,float *x2,float n1,float n2)
{	
    float a,b;
    a = n1;	b =  n2;


    if (a==b)
    {
        if (x1[0]<a) x2[0]=a;
        else if (x1[0]>a) x2[0]=b;
        else x2[0]=a; ///=a?

    }
    else if (a<b)
    {
        if ((x1[0]>a)&&(x1[0]<b))
            x2[0] =x1[0];
        else if (x1[0]>=b) x2[0] = b;
        else if (x1[0]<=a) x2[0] = a;

    }
    else
    {
        if ((x1[0]<a)&&(x1[0]>b))
            x2[0] =x1[0];
        else if (x1[0]<=b) x2[0] = b;
        else if (x1[0]>=a) x2[0] = a;
    }

}

///////////////////////////////
//Dinamicheckie zvenja		 //
///////////////////////////////

void integrator(float *x1,float *x2,float Td)
{
    //*x1 - vhod  zvena  (x1[0] - nactojaschee znachenie x1[1] - pred. znachenie)
    //*x2 - v'ihod zvena (x2[0] - nactojaschee znachenie x2[1] - pred. znachenie)
    //*Td - period dickretizatsii
    //x2[0] = x2[1] + Td[0]/2*(x1[0]+x1[1]);
    //x2[0] = x2[1] + cTd/2*(x1[0]+x1[1]);

    x2[0] = x2[1] + (Td/2)*(x1[1]+x1[0]);           ////////////!!!!!!!!!!!!!!!!
    x2[1] = x2[0];
    x1[1] = x1[0];

}


void biginsmall(float *x,float k_in_small,float zone_width)
{

    if ((x[0] >= zone_width)&&(x[0] <= - zone_width))
    {x[0] = x[0] * k_in_small;}
    else if (x[0] < - zone_width)
    {x[0] = (x[0] + zone_width)- k_in_small*zone_width;}
    else if (x[0] > zone_width)
    {x[0] = (x[0] - zone_width)+ k_in_small*zone_width;}
}

void diff(float *x1, float *x2, float Td)
{

    //    float bx1[2],bx2[2];
    //    bx1[0] = x1[0]; bx1[1] = x1[1];
    //   bx2[0] = x2[0]; bx2[1] = x2[1];

    x2[0] = (1/Td)*(x1[0]-x1[1]);           ////////////!!!!!!!!!!!!!!!!
    x1[1] = x1[0];
}




void ResetAllK(TControlLoop *Loop,int num)
{   

    // Obnulenie vceh H kontura
    int i=0;
    for (i = 0;i < k_max;i++)
    {Loop->k[i] = 0;}
}




void calculate_third_link_position(float *x, float *y, float *z, float *Q0, float *Q1, float *Q2, float *L0, float *L1, float *L2)
{
    Q0[0] = ControlLoop[0].x[2][0]*(ControlLoop[0].k[29]*Kg_r); //Cohranenie cignala c datchika ugla Q0
    Q1[0] = ControlLoop[1].x[2][0]*(ControlLoop[1].k[29]*Kg_r); //Cohranenie cignala c datchika ugla Q1
    Q2[0] = ControlLoop[2].x[2][0]*(ControlLoop[2].k[29]*Kg_r); //Cohranenie cignala c datchika ugla Q2

    x[0] = (L1[0]*cos(Q1[0])+L2[0]*cos(Q2[0]+Q1[0])+L0[0])*cos(Q0[0]);
    z[0] =  L1[0]*sin(Q1[0])+L2[0]*sin(Q2[0]+Q1[0]);
    y[0] = (L1[0]*cos(Q1[0])+L2[0]*cos(Q2[0]+Q1[0])+L0[0])*sin(Q0[0]);

}

void calculate_wrist_position(float *x, float *y, float *z, float *Q0, float *Q1, float *Q2, float *Q3, float *Q4, float *L0, float *L1, float *L2, float *L3, float *L4)
{
    Q0[0] = ControlLoop[0].x[2][0]*(ControlLoop[0].k[29]*Kg_r); //Cohranenie cignala c datchika ugla Q0
    Q1[0] = ControlLoop[1].x[2][0]*(ControlLoop[1].k[29]*Kg_r); //Cohranenie cignala c datchika ugla Q1
    Q2[0] = ControlLoop[2].x[2][0]*(ControlLoop[2].k[29]*Kg_r); //Cohranenie cignala c datchika ugla Q2
    Q3[0] = ControlLoop[3].x[2][0]*(ControlLoop[3].k[29]*Kg_r); //Cohranenie cignala c datchika ugla Q3

    if ((NetD[0] == 1)&&(NetD[1] == 1)) //Right Manipulator has different quantity length of links than Left Manipulator
    {
        Q4[0] = ControlLoop[4].x[2][0]*(ControlLoop[4].k[29]*Kg_r); //Cohranenie cignala c datchika ugla Q4
        x[0] = (L1[0]*cos(Q1[0])+L2[0]*cos(Q2[0]+Q1[0])+L3[0]*cos(Q2[0]+Q3[0]+Q1[0])+L0[0])*cos(Q0[0])+L4[0]*cos(Q0[0]+Q4[0]);
        z[0] =  L1[0]*sin(Q1[0])+L2[0]*sin(Q2[0]+Q1[0])+(L3[0]+L4[0]*cos(Q4[0]))*sin(Q2[0]+Q3[0]+Q1[0]);
        y[0] = (L1[0]*cos(Q1[0])+L2[0]*cos(Q2[0]+Q1[0])+L3[0]*cos(Q2[0]+Q3[0]+Q1[0])+L0[0])*sin(Q0[0])+L4[0]*sin(Q0[0]+Q4[0]);

    }
    else
    {
        x[0] = (L1[0]*cos(Q1[0])+L2[0]*cos(Q2[0]+Q1[0])+L3[0]*cos(Q2[0]+Q3[0]+Q1[0])+L0[0])*cos(Q0[0]);
        z[0] =  L1[0]*sin(Q1[0])+L2[0]*sin(Q2[0]+Q1[0])+L3[0]*sin(Q2[0]+Q3[0]+Q1[0]);
        y[0] = (L1[0]*cos(Q1[0])+L2[0]*cos(Q2[0]+Q1[0])+L3[0]*cos(Q2[0]+Q3[0]+Q1[0])+L0[0])*sin(Q0[0]);
    }



}

void find_angles(float *x, float *y, float *z, float  *Q0, float  *Q1, float  *Q2, float *L0,float  *L1, float  *L2)
{
    float temp_fact = 0;
    float x1;
    Q0[0] = atan(y[0]/x[0]);
    x1 = x[0]/cos(Q0[0])-L0[0];
    temp_fact = z[0]*z[0]+x1*x1;
    if ((x1 >=0)&&(z[0]>=0)) Q1[0] = atan(z[0]/x1)+acos((L1[0]*L1[0]+temp_fact-L2[0]*L2[0])/2/L1[0]/sqrt(temp_fact));
    else if ((x1 < 0)&&(z[0]>0)) Q1[0] =M_PI+atan(z[0]/x1)+acos((L1[0]*L1[0]+temp_fact-L2[0]*L2[0])/2/L1[0]/sqrt(temp_fact));
    else if ((x1 < 0)&&(z[0]<0)) Q1[0] =- M_PI+atan(z[0]/x1)+acos((L1[0]*L1[0]+temp_fact-L2[0]*L2[0])/2/L1[0]/sqrt(temp_fact));
    else Q1[0] = atan(z[0]/x1)+acos((L1[0]*L1[0]+temp_fact-L2[0]*L2[0])/2/L1[0]/sqrt(temp_fact));
    Q2[0] =  acos((L2[0]*L2[0]+L1[0]*L1[0]-temp_fact)/2/L2[0]/L1[0])- M_PI; //Q2 is always negative


}




