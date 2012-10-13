#ifndef __GUARD_OPCODES__
#define __GUARD_OPCODES__

#define NUM_OPCODES 35

enum {
        SYS_addr = 0,           //0nnn
        CLS,                    //0E00
        RET,                    //00EE
        JP_addr,                //1nnn
        CALL_addr,              //2nnn
        SE_Vx_byte,             //3xkk
        SNE_Vx_byte,            //4xkk
        SE_Vx_Vy,               //5xy0
        LD_Vx_byte,             //6xkk
        ADD_Vx_byte,            //7xkk
        LD_Vx_Vy,               //8xy0
        OR_Vx_Vy,               //8xy1
        AND_Vx_Vy,              //8xy2
        XOR_Vx_Vy,              //8xy3
        ADD_Vx_Vy,              //8xy4
        SUB_Vx_Vy,              //8xy5
        SHR_Vx,                 //8xy6
        SUBN_Vx_Vy,             //8xy7
        SHL_Vx,                 //8xyE
        SNE_Vx_Vy,              //9xy0
        LD_I_addr,              //Annn
        JP_V0_addr,             //Bnnn
        RND_Vx_byte,            //Cxkk
        DRW_Vx_Vy_nibble,       //Dxyn
        SKP_Vx,                 //Ex9E
        SKNP_Vx,                //ExA1
        LD_Vx_DT,               //Fx07
        LD_Vx_K,                //Fx0A
        LD_DT_Vx,               //Fx15
        LD_ST_Vx,               //Fx18
        ADD_I_Vx,               //Fx1E
        LD_F_Vx,                //Fx29
        LD_B_Vx,                //Fx33
        LD_memI_Vx,             //Fx55
        LD_Vx_memI              //Fx65
}

#endif //__GUARD_OPCODES__