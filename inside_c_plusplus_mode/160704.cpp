#include <stdio.h>

class baseA
{
    public:
        //virtual ~baseA() { }
        virtual void pri() { printf("baseA\n"); }
};

class drivedB : public baseA
{
    public:
        // virtual ~drivedB() { }
        // virtual void bri() { }
        virtual void pri() 
        { 
            printf("-b::pri %d\n", &baseA::pri);
            baseA::pri();
            printf("drivedB\n"); 
        }
};

class drivedc : public drivedB 
{
    public:
        // virtual ~drivedB() { }
        // virtual void bri() { }
        virtual void pri() 
        { 
            printf("-c::pri: %d\n", &drivedB::pri);
            drivedB::pri();
            printf("drivedC\n"); 
        }
};

int main(int argc, char* argv[])
{
    drivedB dri;
    drivedc driC;
    // baseA *ba = &dri;
    baseA *ba = &driC;
    
    //void (drivedB::*dPtr)() = &drivedB::pri;

    printf("b::D: %d\n", &baseA::pri);
    printf("d::D: %d\n", &drivedB::pri);
    printf("d2::D: %d\n", &drivedc::pri);

    // printf("D::D: %d\n", &drivedB::~drivedB);
    //printf("dPrt: %d\n", dPtr);
    //printf("bri: %d\n", &drivedB::bri);
    ba->pri();

    return 0;
}
