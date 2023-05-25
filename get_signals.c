
// Zanesenie signalov upravlenia iz portov v konturi
void GetSignals(void)
{
    unsigned char num;
    float buf_x1;
    unsigned char Pnum = 0;

    if (ControlLoop[6].k[57] == 1) return;
    //4tenie iz koef-v
    JoyCfg.d_zone[0] = ControlLoop[6].k[15];JoyCfg.n_zone[0] = ControlLoop[6].k[19];JoyCfg.k[0] 	 = ControlLoop[6].k[23];JoyCfg.zero[0] 	 = ControlLoop[6].k[27];JoyCfg.sign[0] 	 = ControlLoop[6].k[31];
    JoyCfg.d_zone[1] = ControlLoop[6].k[16];JoyCfg.n_zone[1] = ControlLoop[6].k[20];JoyCfg.k[1] 	 = ControlLoop[6].k[24];JoyCfg.zero[1] 	 = ControlLoop[6].k[28];JoyCfg.sign[1] 	 = ControlLoop[6].k[32];
    JoyCfg.d_zone[2] = ControlLoop[6].k[17];JoyCfg.n_zone[2] = ControlLoop[6].k[21];JoyCfg.k[2] 	 = ControlLoop[6].k[25];JoyCfg.zero[2] 	 = ControlLoop[6].k[29];JoyCfg.sign[2] 	 = ControlLoop[6].k[33];
    JoyCfg.d_zone[3] = ControlLoop[6].k[18];JoyCfg.n_zone[3] = ControlLoop[6].k[22];JoyCfg.k[3] 	 = ControlLoop[6].k[26];JoyCfg.zero[3] 	 = ControlLoop[6].k[30];JoyCfg.sign[3] 	 = ControlLoop[6].k[34];
    JoyCfg.sign[4] = ControlLoop[6].k[35]; JoyCfg.sign[5] = ControlLoop[6].k[36]; JoyCfg.sign[6] = ControlLoop[6].k[52];
    //signali s prop kanalov
    if ((prev_arm_status == ARM_RAB_STEPEN)||(prev_arm_status == ARM_RAB_PRESS)||
            (prev_arm_status == ARM_RAZOMKN	 )||(prev_arm_status == ARM_RAB_PLOSK)||
            (prev_arm_status == ARM_RAB_XYZ   ))
    {
        //Rukoyatka vlevo-vpravo
        num = 0;

        t123 = (float)(nRecvMain.joy_y/(32767.0/5.0));

        buf_x1 = JoyCfg.k[num]*(t123-JoyCfg.zero[num]);

        //			buf_x1 = JoyCfg.k[num]*(Hrdwr.Vin[7]-JoyCfg.zero[num]);
        dead_zone_j(&buf_x1,&buf_x1,JoyCfg.d_zone[num]);
        //vozvedenie signala v kvadrat s u4etom znaka
        if (buf_x1<0) buf_x1 = -1*buf_x1*buf_x1; else buf_x1 = buf_x1*buf_x1;

        n_zone_j(&buf_x1,&buf_x1,JoyCfg.n_zone[num]);


        if (nRecvMain.joy_function == 1)
        {

            ControlLoop[2].x[1][0] = JoyCfg.sign[num]*buf_x1;
            ControlLoop[4].x[1][0] = 0.0;
            Vy = ControlLoop[6].k[39]*buf_x1;

        }
        else
        {
            ControlLoop[2].x[1][0] = 0.0;
            ControlLoop[4].x[1][0] = JoyCfg.sign[4]*buf_x1;
        }//if (Hrdwr.D[ 4] == 1)


        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////
        //Rukoyatka vpered-nazad
        num = 1;
        //			buf_x1 = JoyCfg.k[num]*(Hrdwr.Vin[5]-JoyCfg.zero[num]);

        t123 = (float)(nRecvMain.joy_x/(32767.0/5.0));

        buf_x1 = JoyCfg.k[num]*(t123-JoyCfg.zero[num]);

        dead_zone_j(&buf_x1,&buf_x1,JoyCfg.d_zone[num]);
        //vozvedenie signala v kvadrat s u4etom znaka
        if (buf_x1<0) 	buf_x1 = -1*buf_x1*buf_x1; else	buf_x1 = buf_x1*buf_x1;
        ////
        n_zone_j (&buf_x1,&buf_x1,JoyCfg.n_zone[num]);


        if (nRecvMain.joy_function == 1)
        {

            ControlLoop[1].x[1][0] = JoyCfg.sign[num]*buf_x1;
            ControlLoop[3].x[1][0] = 0.0;
            Vx = ControlLoop[6].k[41]*buf_x1;


        }
        else
        {
            ControlLoop[1].x[1][0] = 0.0;
            ControlLoop[3].x[1][0] = JoyCfg.sign[5]*buf_x1;
        }//if (Hrdwr.D[ 4] == 1)


        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////
        //Rukoyatka mahovik
        num = 2;
        //			buf_x1 = JoyCfg.k[num]*(Hrdwr.Vin[6]-JoyCfg.zero[num]);

        t123 = (float)(nRecvMain.sxvat_intension/(32767.0/5.0));

        buf_x1 = JoyCfg.k[num]*(t123-JoyCfg.zero[num]);
        dead_zone_j(&buf_x1,&buf_x1,JoyCfg.d_zone[num]);
        //vozvedenie signala v kvadrat s u4etom znaka
        if (buf_x1<0) buf_x1 = -1*buf_x1*buf_x1; else buf_x1 = buf_x1*buf_x1;
        ////
        n_zone_j (&buf_x1,&buf_x1,JoyCfg.n_zone[num]);
        //if ((prev_arm_status != ARM_RAB_PLOSK)&&(prev_arm_status != ARM_RAB_XYZ))

        ControlLoop[6].x[1][0] = JoyCfg.sign[num]*buf_x1;
        //	else


        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////
        //Rukoyatka povorot
        num = 3;
        //			buf_x1 = JoyCfg.k[num]*(Hrdwr.Vin[4]-JoyCfg.zero[num]);

        t123 = (float)(nRecvMain.joy_z/(32767.0/5.0));

        buf_x1 = JoyCfg.k[num]*(t123-JoyCfg.zero[num]);
        dead_zone_j(&buf_x1,&buf_x1,JoyCfg.d_zone[num]);
        //vozvedenie signala v kvadrat s u4etom znaka
        if (buf_x1<0) buf_x1 = -1*buf_x1*buf_x1; else	buf_x1 = buf_x1*buf_x1;
        ////
        n_zone_j (&buf_x1,&buf_x1,JoyCfg.n_zone[num]);

        if (nRecvMain.joy_function == 1)
        {

            ControlLoop[0].x[1][0] = JoyCfg.sign[num]*buf_x1;
            ControlLoop[5].x[1][0] = 0.0;
            Vz = ControlLoop[6].k[42]*buf_x1;
        }
        else
        {
            ControlLoop[0].x[1][0] = 0.0;
            ControlLoop[5].x[1][0] = JoyCfg.sign[6]*buf_x1;
        }
        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////

        //Obrabotka knopok rukoyatki
        //Knopki ka4aniya kisti
        if (Hrdwr.D[ 2] == 0)
        {
            if ((prev_arm_status == ARM_RAB_PLOSK)||(prev_arm_status == ARM_RAB_XYZ))
                //ControlLoop[4].x[1][0] = ControlLoop[4].x[1][0]+ControlLoop[6].k[37];
                p4x1 = p4x1 + ControlLoop[6].k[37];
            else
                //Vrashenie INSTRUMENTa
                ControlLoop[6].x[4][0] =  ControlLoop[6].k[2];

        }

        if (Hrdwr.D[ 3] == 0)
        {
            if ((prev_arm_status == ARM_RAB_PLOSK)||(prev_arm_status == ARM_RAB_XYZ))
                //ControlLoop[4].x[1][0] = ControlLoop[4].x[1][0]-ControlLoop[6].k[37];
                p4x1 = p4x1 - ControlLoop[6].k[37];
            else
            {
                //Vrashenie INSTRUMENTa
                if (ControlLoop[6].x[4][0] == 0)
                    ControlLoop[6].x[4][0] = -ControlLoop[6].k[2];
            }
        }

        if ((Hrdwr.D[ 2] == 1)&&(Hrdwr.D[ 3] == 1))
        {
            //Ostanovka INSTRUMENTa
            ControlLoop[6].x[4][0] = 0.0;
        }
        //			}
        //Knopki rotatsii shvata
        if (Hrdwr.D[ 0] == 0)
        {
            if ((prev_arm_status == ARM_RAB_PLOSK)||(prev_arm_status == ARM_RAB_XYZ))
                p5x1 = p5x1 + ControlLoop[6].k[38];
        }
        if (Hrdwr.D[ 1] == 0)
        {
            if ((prev_arm_status == ARM_RAB_PLOSK)||(prev_arm_status == ARM_RAB_XYZ))
                p5x1 = p5x1 - ControlLoop[6].k[38];
        }

        //Knopka STOP
        /*			  	if (nRecvMain.joy_function == 0)  //(Hrdwr.D[4] == 0)
                {
                    if ((prev_arm_status == ARM_RAB_PLOSK)||(prev_arm_status == ARM_RAB_XYZ))
                    {
                        ControlLoop[4].x[1][0] = 0.0;
                        ControlLoop[5].x[1][0] = 0.0;
                        Privod4_x1 = 0.0;
                        p4x1 = 0.0;
                        p5x1 = 0.0;
                        ControlLoop[4].x[5][0] = 0.0;
                        ControlLoop[5].x[5][0] = 0.0;
                    }
                }*///RDyabirov

        //				if (ControlLoop[6].k[49] == 1)
        //					tool = nRecvMain.instrum_cur;
        //				else
        //					tool = 0;
        if (tool == 3) tool = 0; //esli nistrument sbroshen - instrumenta prosto net
        //Esli instrument Shetka ili No* i tumbler opushen vniz
        if (((tool == 1)||(tool == 2))&&(Hrdwr.D[ 7]==0)&&(Hrdwr.D[ 4] == 1))
            ControlLoop[6].x[4][0] =  ControlLoop[6].k[2];
        //Esli instrument Shetka ili No* i tumbler podnat vverh
        if (((tool == 1)||(tool == 2))&&(Hrdwr.D[ 5]==0)&&(Hrdwr.D[ 4] == 1))
            ControlLoop[6].x[4][0] = -ControlLoop[6].k[2];

    }//if ((prev_arm_status == ARM_RAB_STEPEN)||

    //Upravlenie otkritiem-zakritiem sxvata bez instrimenta i ostanovkoy dvi*eniya sxvata i instrumenta
    //vineseno otdelno,t.k.sxvat nado umet otkrivat v ru4nuu i pri vistavke sxvata v avtomati4eskom re*ime
    if ((prev_arm_status == ARM_RAB_STEPEN)||
            (prev_arm_status == ARM_RAB_PRESS)||
            (prev_arm_status == ARM_RAZOMKN)||
            (prev_arm_status == ARM_RAB_PLOSK)||
            (prev_arm_status == ARM_RAB_XYZ)||
            (TrajMovePause	== 1))
    {
        //Esli net instrumenta i tumbler opushen vniz i ne na*ata knopka stop
        if (((tool == 0)&&(Hrdwr.D[ 7]==0)&&(Hrdwr.D[4] == 1))||
                ((Hrdwr.D[ 7]==0)&&(TrajMovePause == 1)))
        {
            //ControlLoop[6].x[1][0] =  5.0;
            //ControlLoop[6].x[5][0] =  5.0;
            ///ControlLoop[6].x[1][0] = ControlLoop[6].k[44];
        }
        //Esli net instrumenta i tumbler podnat vverh i ne na*ata knopka stop
        if (((tool == 0)&&(Hrdwr.D[ 5]==0)&&(Hrdwr.D[4] == 1))||
                ((Hrdwr.D[ 5]==0)&&(TrajMovePause == 1)))
        {
            ///ControlLoop[6].x[1][0] = ControlLoop[6].k[43];
        }
        //tumbler v neytralnom polo*enii - ostanovit dvi*enie Sxvata
        if ((Hrdwr.D[5] == 1)&&(Hrdwr.D[7] == 1))
        {
            ///ControlLoop[6].x[1][0] =  0.0;
            ControlLoop[6].x[3][0] =  0.0;
            ControlLoop[6].x[5][0] =  0.0;
        }
    }

    if (ControlLoop[6].k[50] == 1)
    {
        Pnum  	= (unsigned char)ControlLoop[6].k[51];
        ControlLoop[Pnum].x[1][0] = nRecvMVTU.Signal;
    }

    if (ControlLoop[6].k[50] == 2)
    {
        ControlLoop[Pnum].x[1][0] = 0.0;
        Pnum  	= (unsigned char)ControlLoop[6].k[51];
        ControlLoop[Pnum].x[12][0] = nRecvMVTU.Signal;
        ControlLoop[Pnum].x[12][1] = nRecvMVTU.Signal;
    }

    //////Kompensatsiya inversii

    if (prev_arm_status != ARM_RAZOMKN)
    {
        ControlLoop[0].x[1][0] = ControlLoop[0].k[6]*ControlLoop[0].x[1][0];
        ControlLoop[1].x[1][0] = ControlLoop[1].k[6]*ControlLoop[1].x[1][0];
        ControlLoop[2].x[1][0] = ControlLoop[2].k[6]*ControlLoop[2].x[1][0];
        ControlLoop[3].x[1][0] = ControlLoop[3].k[6]*ControlLoop[3].x[1][0];
        ControlLoop[4].x[1][0] = ControlLoop[4].k[6]*ControlLoop[4].x[1][0];
        ControlLoop[5].x[1][0] = ControlLoop[5].k[6]*ControlLoop[5].x[1][0];
    }
}

//////////////////////////////////////////////////////
// Zanesenie rezultatov rass4eta kontura v porti
void SetSignals(void)
{
    if (ControlLoop[6].x[5][0]< 0) SXVAT_Vout = ControlLoop[6].k[3];
    if (ControlLoop[6].x[5][0]>=0) SXVAT_Vout = ControlLoop[6].k[4];

    Hrdwr.Vout[PrivodDACnum[0]] = ControlLoop[0].x[5][0] + ControlLoop[0].k[21];	//EGU1 - privod 1
    Hrdwr.Vout[PrivodDACnum[1]] = ControlLoop[1].x[5][0] + ControlLoop[1].k[21]; 	//EGU2 - privod 2
    Hrdwr.Vout[PrivodDACnum[2]] = ControlLoop[2].x[5][0] + ControlLoop[2].k[21];	//EGU3 - privod 3
    Hrdwr.Vout[PrivodDACnum[3]] = ControlLoop[3].x[5][0] + ControlLoop[3].k[21];	//EGU4 - privod 4
    Hrdwr.Vout[PrivodDACnum[4]] = ControlLoop[4].x[5][0] + ControlLoop[4].k[21];	//EGU5 - privod 5
    Hrdwr.Vout[PrivodDACnum[5]] = ControlLoop[5].x[5][0] + ControlLoop[5].k[21];	//EGU6 - privod 6 rotatsiya SXVATa
    Hrdwr.Vout[PrivodDACnum[6]] = ControlLoop[6].x[5][0] + ControlLoop[6].k[0];	//EGU7 - privod 6 SXVATa open/close
    //Logika dla vibora napra*eniya na SXVATe
    //Dla uder*aniya instrumenta esli on v SXVATe
    if (ControlLoop[6].k[49] == 1)
    {
        if (((tool == 1)||(tool == 2))&&(prev_arm_status != ARM_INSTR_THROW)&&(prev_prev_arm_status != ARM_INSTR_THROW))
            ControlLoop[6].x[5][0] = ControlLoop[6].k[44]; //napra*enie dla zakritiya SXVATa
    }
    //Dla podder*aniya otkritogo-zakritogo sxvata pri vzatii/vozvrate instrumenta
    if ((prev_arm_status==ARM_BRI_BRUSH)||(prev_arm_status == ARM_BRI_ROPECUT)||
            (prev_arm_status==ARM_RTN_BRUSH)||(prev_arm_status == ARM_RTN_ROPECUT))
        ControlLoop[6].x[5][0] = V_InstrSXVAT;

    //		Hrdwr.Vout[PrivodDACnum[7]] = ControlLoop[6].x[5][0] + SXVAT_Vout;	//EGU8 - privod SXVATa
    //		Hrdwr.Vout[PrivodDACnum[8]] = ControlLoop[6].x[4][0] + ControlLoop[6].k[ 5];	//EGU9 - privod instrumenta

    Hrdwr.Vout[7] = ((float)(nRecvMain.tool_1/10));	//EGU8 - instr1
    Hrdwr.Vout[8] = ((float)(nRecvMain.tool_2/10));	//EGU9 - instr2

    Hrdwr.Vout[11] = (((float)(nRecvMain.light_value)*5)/65536);
    //test sxvat
    Hrdwr.Vout[10] = ControlLoop[6].x[1][0];

    Hrdwr.Vout[12] = 10*ControlLoop[curDrive].x[nRecvTech.xNumber[0]][0];
    Hrdwr.Vout[13] = 10*ControlLoop[curDrive].x[nRecvTech.xNumber[1]][0];
    Hrdwr.Vout[14] = 10*ControlLoop[curDrive].x[nRecvTech.xNumber[2]][0];
    Hrdwr.Vout[15] = 10*ControlLoop[curDrive].x[nRecvTech.xNumber[3]][0];


}
