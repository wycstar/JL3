/* ctors and dtors arrays -- to be used by runtime system */
/*   to call static constructors and destructors          */
/*                                                        */
/* NOTE: Use a C compiler to compile this file. If you    */
/*       are using GNU C++, be sure to use compile this   */
/*       file using a GNU compiler with the               */
/*       -fdollars-in-identifiers flag.                   */


#if !__GNUC__
#error GNU C++ code must be munched and linked using the GNU toolchain
#endif

extern void __register_frame_info (const void *, void *);
extern void *__deregister_frame_info (const void *);
extern const unsigned __EH_FRAME_BEGIN__[];
static void _STI__15_ctors ()
{
  /* This structure must be no smaller than that in unwind-dwe-fde.c */
  static struct object {unsigned pad[8];} object;
  __register_frame_info (__EH_FRAME_BEGIN__, &object);
}

static void _STD__15_dtors ()
{
  __deregister_frame_info (__EH_FRAME_BEGIN__);
}


void _GLOBAL__I__ZN2GV16REMOTE_CTRL_BAUDE();

void _GLOBAL__I_can();

void _GLOBAL__I_control();

void _GLOBAL__I_digitalIO();

void _GLOBAL__I_file();

void _GLOBAL__I_main();

void _GLOBAL__I_net();

void _GLOBAL__I_ui();

extern void (*_ctors[])();
void (*_ctors[])() =
    {
    _STI__15_ctors,
    _GLOBAL__I__ZN2GV16REMOTE_CTRL_BAUDE,
    _GLOBAL__I_can,
    _GLOBAL__I_control,
    _GLOBAL__I_digitalIO,
    _GLOBAL__I_file,
    _GLOBAL__I_main,
    _GLOBAL__I_net,
    _GLOBAL__I_ui,
    0
    };

void _STD__15_dtors();

void _GLOBAL__D__ZN2GV16REMOTE_CTRL_BAUDE();

void _GLOBAL__D_can();

void _GLOBAL__D_control();

void _GLOBAL__D_digitalIO();

void _GLOBAL__D_file();

void _GLOBAL__D_main();

void _GLOBAL__D_net();

void _GLOBAL__D_ui();

extern void (*_dtors[])();
void (*_dtors[])() =
    {
    _STD__15_dtors,
    _GLOBAL__D__ZN2GV16REMOTE_CTRL_BAUDE,
    _GLOBAL__D_can,
    _GLOBAL__D_control,
    _GLOBAL__D_digitalIO,
    _GLOBAL__D_file,
    _GLOBAL__D_main,
    _GLOBAL__D_net,
    _GLOBAL__D_ui,
    0
    };
