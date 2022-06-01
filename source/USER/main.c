#include "stm32f4xx.h"

#include "board.h"

int main()
{
    System_BspInit();
    System_SateInit();

    while(1)
    {
        if(ReceiveFrame() == SUCCESS)
        {
            CmdDealProcess();
        }
        
        Main_Loop_Task();
    }
}
