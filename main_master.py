import cv2
import numpy as np
import serial
# counter variables
i=0
bl=[]
bc1=0
bc2=0
ctr=[]
ctr2=[]
ct=[]
tmp=[]
br=0
count2=0
count=0
def detect_boulders(im):                                        ## this function detects the numbers  present on boulders to be pickes and placed
    for i in range(0,10):
        tmp[i]=cv2.imread(str(i)+'.jpg')
#    m1=im[40:240,375:500]
 #   m2=im[40:240,500:640]
  #  m3=im[240:480,375:500]
  #  m4=im[240:480,500:640]
    for i in range(0,10):
        res = cv2.matchTemplate(img,img3,cv2.TM_SQDIFF)
        min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
        x,y=min_loc
        if((x<375 )&( y<240)& (x>112)):
            bz3=i
            
        elif((x>375 )&( y<240)& (x>112)):
            bz4=i
        elif((x>375 )&( y>240)& (x>112)):
            bz1=i
        elif((x<375 )&( y>240)& (x>112)):
            bz2=i
        bl=[bz1,bz2,bz3,bz4];
def analyse_sum(n):    # this function analyses the sum and shows which boulders are to be picked up
    
    i=9
    j=9
    q=9
    w=9
    for i in range(0,5):
        sm=bl[i]
        for j in range(i,n):
            sm=bl[i]+bl[j]
            f1=1
            f2=0
            f3=0
            for q in range(j,n):
                sm=bl[i]+bl[j]+bl[q]
                f2=1
                f2=0
                f3=0
                for w in range(q,n):
                    sm=bl[i]+bl[j]+bl[q]+bl[w]
                    f3=1
                    f1=0
                    f2=0
        if(sm==smm):
            return 1
        else:
            return 0
def nvb1b(im):            ##this function navigates the robot in bridge one in backward direction i.e towards crater region
    while (bobf==1):
        bobf=0
        img1=im[50:200,112:372]
        gray = cv2.cvtColor(img1,cv2.COLOR_BGR2GRAY)
        ret,thresh = cv2.threshold(gray,127,255,0)
        contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
        
    ##    cv2.drawContours(img1,contours,-1,(0,255,0),3)
        for i in range (len(contours)):
            
            Area = cv2.contourArea(contours[i])
            print Area
            if ((Area>=750) & (Area<1000)):
                bobf=1
                M = cv2.moments(contours[i])
                bcx = int(M['m10']/M['m00'])
                bcy = int(M['m01']/M['m00'])
              ##  bct.append((cx+112,cy+260));
                cv2.circle(img,(cx+112,cy+260), 1, (225,0,255), -1)
        
        cx,cy=ctr[count]
        ths= cx+54
        ths1=ths-13
        #send data for navigaton to slave
        if((cy==bcy) & (bcx<=ths)& (bcx>=ths1)):
            xcc.write('8')
        if((cy==bcy) & (bcx>ths1)| (bcx>ths) ):
            xcc.write('8')
        if(bcy>cy):
            xcc.write('4')
            
            xcc.write('2')
            xcc.write('6')
            
        if(bcy>cy):
            xcc.write('6')
            
            xcc.write('2')
            xcc.write('4')

        

def navigate_bridge1(im):           # this function navigates the robot throug bridge 1
    img1=im[50:200,112:372]
    gray = cv2.cvtColor(img1,cv2.COLOR_BGR2GRAY)
    ret,thresh = cv2.threshold(gray,127,255,0)
    contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
   
##    cv2.drawContours(img1,contours,-1,(0,255,0),3)
    for i in range (len(contours)):
        
        Area = cv2.contourArea(contours[i])
        print Area
        if ((Area>=750) & (Area<1000)):
            bof=1
            M = cv2.moments(contours[i])
            bcx = int(M['m10']/M['m00'])##calculate centroid of robot
            bcy = int(M['m01']/M['m00'])
          ##  bct.append((cx+112,cy+260));
            cv2.circle(img,(cx+112,cy+260), 1, (225,0,255), -1)

    cx,cy=ctr[count]
    ths= cx+54
    ths1=ths-13
    #send data for navigaton to slave
    if((cy==bcy) & (bcx<=ths)& (bcx>=ths1)) :
        count=count + 1
        xcc.write('r')
        if(count==len(ctr)):
            for a in range(0,27):
                xcc.write('8')

        else:
            xcc.write('p')
            bof=0
            nvb1b(img)
    if((cy==bcy) & (bcx<=ths)& (bcx<=ths1)):
        xcc.write('2')
    if((cy==bcy) & (bcx>ths)):
        xcc.write('8')
    if(bcy>cy):
        xcc.write('6')
        
        xcc.write('2')
        xcc.write('4')
        
    if(bcy>cy):
        xcc.write('4')
        
        xcc.write('2')
        xcc.write('6')
    if((cy==bcy) & (bcx<ths)):
        xcc.write('8')
def nvb2b(im):          ##this function navigates the robot in bridge one in backward direction i.e towards crater region
    while (bobf==1):
        bobf=0
        img1=im[50:200,112:372]
        gray = cv2.cvtColor(img1,cv2.COLOR_BGR2GRAY)
        ret,thresh = cv2.threshold(gray,127,255,0)
        contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
        count=0
    ##    cv2.drawContours(img1,contours,-1,(0,255,0),3)
        for i in range (len(contours)):
            
            Area = cv2.contourArea(contours[i])
            print Area
            if ((Area>=750) & (Area<1000)):
                bobf=1
                M = cv2.moments(contours[i])
                bcx = int(M['m10']/M['m00'])## calculate centroid of robot
                bcy = int(M['m01']/M['m00'])
              ##  bct.append((cx+112,cy+260));
                cv2.circle(img,(cx+112,cy+260), 1, (225,0,255), -1)
                #send data for navigaton to slave
        for a in range(0,len(ct)):
            ocx,ocy=ct[a]
            if((bcx>ocx+30)&(bcy>= ocy+10)):
                xcc.write('4')
                xcc.write('4')
                xcc.write('4')
                xcc.write('4')
                xcc.write('4')
                xcc.write('4')
                for j in range(0,10):
                    xcc.write('8')
                xcc.write('6')
                xcc.write('6')
                xcc.write('6')
                xcc.write('6')
                xcc.write('6')
                xcc.write('6')
               
            elif((bcx>ocx+30)&(bcy<= ocy+10)):
                xcc.write('6')
                xcc.write('6')
                xcc.write('6')
                xcc.write('6')
                xcc.write('6')
                xcc.write('6')
                for j in range(0,10):
                    xcc.write('8')
                xcc.write('4')
                xcc.write('4')
                xcc.write('4')
                xcc.write('4')
                xcc.write('4')
                xcc.write('4')

            
            

        cx,cy=ctr[count]
        ths= cx+54
        ths1=ths-13
        if((cy==bcy) & (bcx<=ths)& (bcx>=ths1)):
            xcc.write('8')
        if((cy==bcy) & (bcx>ths1)| (bcx>ths) ):
            xcc.write('8')
        if(bcy>cy):
            xcc.write('4')
            
            xcc.write('2')
            xcc.write('6')
            
        if(bcy>cy):
            xcc.write('6')
            
            xcc.write('2')
            xcc.write('4')

        

def navigate_bridge2(im):               # this function navigates the robot throug bridge 2
    img1=im[300:450,112:400]
    gray = cv2.cvtColor(img1,cv2.COLOR_BGR2GRAY)
    ret,thresh = cv2.threshold(gray,127,255,0)
    contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    
##    cv2.drawContours(img1,contours,-1,(0,255,0),3)
    for i in range (len(contours)):
        
        Area = cv2.contourArea(contours[i])
        print Area
        if ((Area>=750) & (Area<1000)):
            bof2=1
            M = cv2.moments(contours[i])
            bcx = int(M['m10']/M['m00'])      ## centroid of robot
            bcy = int(M['m01']/M['m00'])
          ##  bct.append((cx+112,cy+260));
            cv2.circle(img,(cx+112,cy+260), 1, (225,0,255), -1)
    if(fl==0):
        cx,cy=ctr2[count2]
        
        ths= cx+54
        ths1=ths-13
    else:
        cx=0
        cy=330
    for a in range(0,len(ct)):
        ocx,ocy=ct[a]
        if((bcx>ocx+30)&(bcy>= ocy+10)):
            xcc.write('6')
            xcc.write('6')
            xcc.write('6')
            xcc.write('6')
            xcc.write('6')
            xcc.write('6')
            for j in range(0,10):
                xcc.write('8')
            xcc.write('4')
            xcc.write('4')
            xcc.write('4')
            xcc.write('4')
            xcc.write('4')
            xcc.write('4')
        elif((bcx>ocx+30)&(bcy<= ocy+10)):
            xcc.write('4')
            xcc.write('4')
            xcc.write('4')
            xcc.write('4')
            xcc.write('4')
            xcc.write('4')
            for j in range(0,10):
                xcc.write('8')
            xcc.write('6')
            xcc.write('6')
            xcc.write('6')
            xcc.write('6')
            xcc.write('6')
            xcc.write('6')
            
            
                
            
    if((cy==bcy) & (bcx<=ths)& (bcx>=ths1)) :
        count=count + 1
        xcc.write('r')
        if(count==len(ctr)):
            for a in range(0,27):
                xcc.write('8')
            fl=1

        else:
            xcc.write('p')
            
            bof=0
            nvb2b(img)
    if((cy==bcy) & (bcx<=ths)& (bcx<=ths1)):
        xcc.write('2')
    if((cy==bcy) & (bcx>ths)):
        xcc.write('8')
    if(bcy>cy):
        xcc.write('6')
        
        xcc.write('2')
        xcc.write('4')
        
    if(bcy>cy):
        xcc.write('4')
        
        xcc.write('2')
        xcc.write('6')
    if((cy==bcy) & (bcx<ths)):
        xcc.write('8')
    
def find_obstacles(im):         #this function detects all the obstacles on bridg 2
    img1=im[260:400,112:372]
    gray = cv2.cvtColor(img1,cv2.COLOR_BGR2GRAY)
    ret,thresh = cv2.threshold(gray,127,255,0)
    contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
##    cv2.drawContours(img1,contours,-1,(0,255,0),3)
    for i in range (len(contours)):
        
        Area = cv2.contourArea(contours[i])
        print Area
        if ((Area>=140) & (Area<400)):
            M = cv2.moments(contours[i])
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            ct.append((cx+112,cy+260));
            cv2.circle(img,(cx+112,cy+260), 1, (0,0,255), -1)
##           // n=n+1
    print ct
def find_cavities(im):      # this function finds all the cavities
    img1=im[100:150,112:372]
    img2=im[260:400,112:372]
    gray = cv2.cvtColor(img1,cv2.COLOR_BGR2GRAY)
    ret,thresh = cv2.threshold(gray,127,255,0)
    contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
##    cv2.drawContours(img1,contours,-1,(0,255,0),3)
    for i in range (len(contours)):
        
        Area = cv2.contourArea(contours[i])
        print Area
        if ((Area>=600) & (Area<750)):
            M = cv2.moments(contours[i])
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            ctr.append((cx+112,cy+100));
            cv2.circle(img,(cx+112,cy+100), 1, (0,0,255), -1)
##           n=n+1
    bc1=len(ctr)
    print 'bc1= '+str(bc1)
    gray2 = cv2.cvtColor(img2,cv2.COLOR_BGR2GRAY)
    ret,thresh2 = cv2.threshold(gray2,127,255,0)
    contours2, hierarchy = cv2.findContours(thresh2,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
##    cv2.drawContours(img2,contours2,-1,(0,255,0),3)
##    n=0
    for i in range (len(contours2)):
        
        Area = cv2.contourArea(contours2[i])
        
        print Area
        if ((Area>=600) & (Area<1000)):
            M = cv2.moments(contours2[i])
            cx = int(M['m10']/M['m00'])
            cy = int(M['m01']/M['m00'])
            ctr2.append((cx+112,cy+260))
            cv2.circle(img,(cx+112,cy+260), 1, (0,0,255), -1)
##           // n=n+1
    bc2=len(ctr2)
    print 'bc2= '+str(bc2)
    cv2.imshow('image',img1)
    cv2.imshow('image1',img2)

################################################################
cap = cv2.VideoCapture(1)
ret, img= cap.read()
find_cavities(img)
find_obstacles(img)
detect_boulders(img)
xcc=serial.Serial('COM9',9600)
flg=sum_analyse(len(ctr2))

if (flg==1):
    br=2
    if(f1==0):              ## sending data to firebird i.e boulder numbers the bridge to be traveled and boulders to be picked up
        xcc.write('#'+str(bz1)+str(bz2)+str(bz3)+str(bz4)+'2'+str(bl[i])+'@@@')
    elif(f1==1):
        xcc.write('#'+str(bz1)+str(bz2)+str(bz3)+str(bz4)+'2'+str(bl[i])+str(bl[j])+'@@')
    elif(f2==1):
        xcc.write('#'+str(bz1)+str(bz2)+str(bz3)+str(bz4)+'2'+str(bl[i])+str(bl[j])+str(bl[q])+'@')
    elif(f3==1):
        xcc.write('#'+str(bz1)+str(bz2)+str(bz3)+str(bz4)+'2'+str(bl[i])+str(bl[j])+str(bl[q])+str(bl[w]))
    xcc.write('y')
    while(bof==1):
        bof=0
        ## Read the image
    ret, frame = cap.read()
    navigate_bridge1(frame)
    if (bof==0):
        xcc.write('l')
        
else:
    flg=sum_analyse(len(ctr))
    
    if (flg==1):
        br=2
        if(f1==0):      ## sending data to firebird i.e boulder numbers the bridge to be traveled and boulders to be picked up
            xcc.write('#'+str(bz1)+str(bz2)+str(bz3)+str(bz4)+'2'+str(bl[i])+'@@@')
        elif(f1==1):
            xcc.write('#'+str(bz1)+str(bz2)+str(bz3)+str(bz4)+'2'+str(bl[i])+str(bl[j])+'@@')
        elif(f2==1):
            xcc.write('#'+str(bz1)+str(bz2)+str(bz3)+str(bz4)+'2'+str(bl[i])+str(bl[j])+str(bl[q])+'@')
        elif(f3==1):
            xcc.write('#'+str(bz1)+str(bz2)+str(bz3)+str(bz4)+'2'+str(bl[i])+str(bl[j])+str(bl[q])+str(bl[w]))
        xcc.write('y')
        while(bof2==1):
            bof2=0
            ret, frame = cap.read()
            navigate_bridge2(img)
        if (bof2==0):
            xcc.write('l')
##res = cv2.matchTemplate(img,img3,cv2.TM_SQDIFF)
##min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)

##cv2.circle(img,min_loc, 2, (0,255,255), -1)
##print min_loc
##cv2.imshow('image0',img)

cv2.waitKey(0)
cap.release()
cv2.destroyAllWindows()
