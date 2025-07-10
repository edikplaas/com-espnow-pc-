
def getTemp(rawTemp,ID): # Sert à récupérer la température réelle à partir d'une température brute et de coefficients
    NVM_PAR_T3=-12
    if ID==1:
        NVM_PAR_T1= 27590
        NVM_PAR_T2=18750
    elif ID==3:
        NVM_PAR_T1=27662
        NVM_PAR_T2=18973
    elif ID==5:
        NVM_PAR_T1=27511
        NVM_PAR_T2=18794
    elif ID==7:
        NVM_PAR_T1=27807
        NVM_PAR_T2=19371
    elif ID==9:
        NVM_PAR_T1= 27763
        NVM_PAR_T2=18983
    elif ID==11:
        NVM_PAR_T1=27750
        NVM_PAR_T2=19099
    elif ID==13:
        NVM_PAR_T1=27768
        NVM_PAR_T2=18657
    elif ID==15:
        NVM_PAR_T1=27575
        NVM_PAR_T2=18973
    
    PAR_T1 = NVM_PAR_T1 * (1 << 8)
    PAR_T2 = NVM_PAR_T2 / (1 << 30)
    PAR_T3 = NVM_PAR_T3 / (1 << 48)
    partial_data1_temp = rawTemp - PAR_T1
    partial_data2_temp = partial_data1_temp * PAR_T2
    temp = partial_data2_temp + (partial_data1_temp * partial_data1_temp) * PAR_T3
    return temp

def getPress(rawPress,temp,ID):# Sert à récupérer la pression atmosphérique réelle à partir d'une pression brute et de coefficients
    NVM_PAR_P1 = 230
    NVM_PAR_P2 = -1962
    NVM_PAR_P5 = 25549
    NVM_PAR_P6 = 30655
    NVM_PAR_P9 = 16736
    NVM_PAR_P10 = 15
    NVM_PAR_P3 = 30
    NVM_PAR_P4 = -3
    
    NVM_PAR_P7 = -13
    NVM_PAR_P8 = 3
    
    NVM_PAR_P11 = -59
    if ID==1:
        return 0
    elif ID==3:
        NVM_PAR_P1 = 230
        NVM_PAR_P2 = -1962
        NVM_PAR_P5 = 25549
        NVM_PAR_P6 = 30655
        NVM_PAR_P9 = 16736
        NVM_PAR_P10 = 15
    elif ID==5:
        NVM_PAR_P1 = 122
        NVM_PAR_P2 = -2459
        NVM_PAR_P5 = 25290
        NVM_PAR_P6 = 30667
        NVM_PAR_P9 = 16368
        NVM_PAR_P10 = 20
    elif ID==7:
        NVM_PAR_P1 = -181
        NVM_PAR_P2 = -2349
        NVM_PAR_P5 = 25681
        NVM_PAR_P6 = 30215
        NVM_PAR_P9 = 16210
        NVM_PAR_P10 = 23
    elif ID==9:
        NVM_PAR_P1 = 121
        NVM_PAR_P2 = -2090
        NVM_PAR_P5 = 25250
        NVM_PAR_P6 = 30200
        NVM_PAR_P9 = 16305
        NVM_PAR_P10 = 15
    elif ID==11:
        NVM_PAR_P1 = -358
        NVM_PAR_P2 = -2628
        NVM_PAR_P5 = 25841
        NVM_PAR_P6 = 30585
        NVM_PAR_P9 = 16330
        NVM_PAR_P10 = 23
    elif ID==13:
        NVM_PAR_P1 = 136
        NVM_PAR_P2 = -2770
        NVM_PAR_P5 = 25196
        NVM_PAR_P6 = 30866
        NVM_PAR_P9 = 15861
        NVM_PAR_P10 = 27
    elif ID==15:
        NVM_PAR_P1 = -8
        NVM_PAR_P2 = -2269
        NVM_PAR_P5 = 25551
        NVM_PAR_P6 = 30619
        NVM_PAR_P9 = 16334
        NVM_PAR_P10 = 15
    PAR_P1 = (NVM_PAR_P1 - float(1 << 14)) / float(1 << 20)
    PAR_P2 = (NVM_PAR_P2 - float(1 << 14)) / float(1 << 29)
    PAR_P3 = NVM_PAR_P3 / float(1 << 32)
    PAR_P4 = NVM_PAR_P4 / float(1 << 37)
    PAR_P5 = NVM_PAR_P5 * float(1 << 3)
    PAR_P6 = NVM_PAR_P6 / float(1 << 6)
    PAR_P7 = NVM_PAR_P7 / float(1 << 8)
    PAR_P8 = NVM_PAR_P8 / float(1 << 15)
    PAR_P9 = NVM_PAR_P9 / float(1 << 48)
    PAR_P10 = NVM_PAR_P10 / float(1 << 48)
    PAR_P11 = NVM_PAR_P11 / 36893488147419103232 # 2^65

    # Application de la compensation
    partial_data1 = PAR_P6 * temp
    partial_data2 = PAR_P7 * temp * temp
    partial_data3 = PAR_P8 * temp * temp * temp
    partial_out1 = PAR_P5 + partial_data1 + partial_data2 + partial_data3

    partial_data1 = PAR_P2 * temp
    partial_data2 = PAR_P3 * temp * temp
    partial_data3 = PAR_P4 * temp * temp * temp
    partial_out2 = rawPress * PAR_P1 + partial_data1 + partial_data2 + partial_data3

    partial_data1 = rawPress * rawPress
    partial_data2 = PAR_P9 + PAR_P10 * temp
    partial_data3 = partial_data1 * partial_data2
    partial_data4 = partial_data3 + rawPress * rawPress * rawPress * PAR_P11

    press = partial_out1 + partial_out2 + partial_data4
    return press

'''
# exemple d'appel
ID1=1
temp = getTemp(tempBrute,ID1)
press = getPress(pressionBrute,temp,ID1)
'''