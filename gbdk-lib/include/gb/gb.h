/** @file gb/gb.h
    Gameboy specific functions.
*/
#ifndef _GB_H
#define _GB_H

#include <types.h>
#include <gb/hardware.h>
#include <gb/sgb.h>
#include <gb/cgb.h>

/** Joypad bits.
    A logical OR of these is used in the wait_pad and joypad
    functions.  For example, to see if the B button is pressed
    try

    UINT8 keys;
    keys = joypad();
    if (keys & J_B) {
    	...
    }

    @see joypad
 */
#define	J_START      0x80U
#define	J_SELECT     0x40U
#define	J_B          0x20U
#define	J_A          0x10U
#define	J_DOWN       0x08U
#define	J_UP         0x04U
#define	J_LEFT       0x02U
#define	J_RIGHT      0x01U

/** Screen modes.
    Normally used by internal functions only.
 */
#define	M_DRAWING    0x01U
#define	M_TEXT_OUT   0x02U
#define	M_TEXT_INOUT 0x03U
/** Set this in addition to the others to disable scrolling 
    If scrolling is disabled, the cursor returns to (0,0) */
#define M_NO_SCROLL  0x04U
/** Set this to disable \n interpretation */
#define M_NO_INTERP  0x08U

/** If this is set, sprite colours come from OBJ1PAL.  Else
    they come from OBJ0PAL.
*/
#define S_PALETTE    0x10U
/** If set the sprite will be flipped horizontally.
 */
#define S_FLIPX      0x20U
/** If set the sprite will be flipped vertically.
 */
#define S_FLIPY      0x40U
/** If this bit is clear, then the sprite will be displayed
    ontop of the background and window.
*/
#define S_PRIORITY   0x80U

/* Interrupt flags */
/** Vertical blank interrupt.
    Occurs at the start of the vertical blank.  During this
    period the video ram may be freely accessed.
 */
#define VBL_IFLAG    0x01U
/** Interrupt when triggered by the STAT register.
    See the Pan doc.
*/
#define LCD_IFLAG    0x02U
/** Interrupt when the timer TIMA overflows.
 */
#define TIM_IFLAG    0x04U
/** Occurs when the serial transfer has completed.
 */
#define SIO_IFLAG    0x08U
/** Occurs on a transition of the keypad.
 */
#define JOY_IFLAG    0x10U

/* Limits */
/** Width of the visible screen in pixels.
 */
#define SCREENWIDTH  0xA0U
/** Height of the visible screen in pixels.
 */
#define SCREENHEIGHT 0x90U
#define MINWNDPOSX   0x07U
#define MINWNDPOSY   0x00U
#define MAXWNDPOSX   0xA6U
#define MAXWNDPOSY   0x8FU

/* ************************************************************ */

/** Interrupt handlers
 */
typedef void (*int_handler)(void);

/** The remove functions will remove any interrupt
   handler.  A handler of NULL will cause bad things
   to happen.
*/
void
remove_VBL(int_handler h);

void
remove_LCD(int_handler h);

void
remove_TIM(int_handler h);

void
remove_SIO(int_handler h);

void
remove_JOY(int_handler h);

/** Adds a V-blank interrupt handler.
    The handler 'h' will be called whenever a V-blank
    interrupt occurs.  Up to 4 handlers may be added,
    with the last added being called last.  If the remove_VBL
    function is to be called, only three may be added.
    @see remove_VBL
*/
void
add_VBL(int_handler h);

void
add_LCD(int_handler h);

void
add_TIM(int_handler h);

void
add_SIO(int_handler h);

void
add_JOY(int_handler h);

/* ************************************************************ */

/** Set the current mode - one of M_* defined above */
void
	mode(UINT8 m);

/** Returns the current mode */
UINT8
	get_mode(void);

/** GB type (GB, PGB, CGB) */
extern UINT8 _cpu;

/** Original GB or Super GB */
#define DMG_TYPE 0x01 
/** Pocket GB or Super GB 2 */
#define MGB_TYPE 0xFF
/** Color GB */
#define CGB_TYPE 0x11 

/** Time in VBL periods (60Hz) */
extern UINT16 sys_time;	

/* ************************************************************ */

/** Send byte in _io_out to the serial port */
void
send_byte(void);

/** Receive byte from the serial port in _io_in */
void
receive_byte(void);

/** An OR of IO_* */
extern UINT8 _io_status;
/** Byte just read. */
extern UINT8 _io_in;
/** Write the byte to send here before calling send_byte()
    @see send_byte
*/
extern UINT8 _io_out;

/* Status codes */
/** IO is completed */
#define IO_IDLE		0x00U		
/** Sending data */
#define IO_SENDING	0x01U		
/** Receiving data */
#define IO_RECEIVING	0x02U		
/** Error */
#define IO_ERROR	0x04U		

/* ************************************************************ */

/* Multiple banks */

/** MBC1 */
#define SWITCH_ROM_MBC1(b) \
  *(unsigned char *)0x2000 = (b)

#define SWITCH_RAM_MBC1(b) \
  *(unsigned char *)0x4000 = (b)

#define ENABLE_RAM_MBC1 \
  *(unsigned char *)0x0000 = 0x0A

#define DISABLE_RAM_MBC1 \
  *(unsigned char *)0x0000 = 0x00

/* Note the order used here.  Writing the other way around
 * on a MBC1 always selects bank 0 (d'oh)
 */
/** MBC5 */
#define SWITCH_ROM_MBC5(b) \
  *(unsigned char *)0x3000 = (UINT16)(b)>>8; \
  *(unsigned char *)0x2000 = (UINT8)(b)

#define SWITCH_RAM_MBC5(b) \
  *(unsigned char *)0x4000 = (b)

#define ENABLE_RAM_MBC5 \
  *(unsigned char *)0x0000 = 0x0A

#define DISABLE_RAM_MBC5 \
  *(unsigned char *)0x0000 = 0x00

/* ************************************************************ */

/** Delays the given number of milliseconds.
    Uses no timers or interrupts, and can be called with 
    interrupts disabled (why nobody knows :)
 */
void
delay(UINT16 d);

/* ************************************************************ */

/** Reads and returns the current state of the joypad.
    Follows Nintendo's guidelines for reading the pad.
    Return value is an OR of J_*
    @see J_START
*/
UINT8
joypad(void);

/** Waits until all the keys given in mask are pressed.
    Normally only used for checking one key, but it will
    support many, even J_LEFT at the same time as J_RIGHT :)
    @see joypad, J_START
*/
UINT8
waitpad(UINT8 mask);

/** Waits for the pad and all buttons to be released.
*/
void
waitpadup(void);

/* ************************************************************ */

/** Enables unmasked interrupts
    @see disable_interrupts
*/
void
enable_interrupts(void);

/** Disables interrupts.
    This function may be called as many times as you like;
    however the first call to enable_interrupts will re-enable
    them.
    @see enable_interrupts
*/
void
disable_interrupts(void);

/** Clears any pending interrupts and sets the interrupt mask
    register IO to flags.
    @see VBL_IFLAG
    @param flags	A logical OR of *_IFLAGS
*/
void
set_interrupts(UINT8 flags);

/** Performs a warm reset by reloading the CPU value
    then jumping to the start of crt0 (0x0150)
*/
void
reset(void);

/** Waits for the vertical blank interrupt (VBL) to finish.  
    This can be used to sync animation with the screen 
    re-draw.  If VBL interrupt is disabled, this function will
    never return.  If the screen is off this function returns
    immediatly.
*/
void
wait_vbl_done(void);

/** Turns the display off.
    Waits until the VBL interrupt before turning the display
    off.
    @see DISPLAY_ON
*/
void
display_off(void);

/* ************************************************************ */

/** Copies data from somewhere in the lower address space
    to part of hi-ram.
    @param dst		Offset in high ram (0xFF00 and above)
    			to copy to.
    @param src		Area to copy from
    @param n		Number of bytes to copy.
*/
void
hiramcpy(UINT8 dst,
	 const void *src,
	 UINT8 n);

/* ************************************************************ */

/** Turns the display back on.
    @see display_off, DISPLAY_OFF
*/
#define DISPLAY_ON \
  LCDC_REG|=0x80U

/** Turns the display off immediatly.
    @see display_off, DISPLAY_ON
*/
#define DISPLAY_OFF \
  display_off();

#define SHOW_BKG \
  LCDC_REG|=0x01U

#define HIDE_BKG \
  LCDC_REG&=0xFEU

#define SHOW_WIN \
  LCDC_REG|=0x20U

#define HIDE_WIN \
  LCDC_REG&=0xDFU

#define SHOW_SPRITES \
  LCDC_REG|=0x02U

#define HIDE_SPRITES \
  LCDC_REG&=0xFDU

#define SPRITES_8x16 \
  LCDC_REG|=0x04U

#define SPRITES_8x8 \
  LCDC_REG&=0xFBU

/* ************************************************************ */

void
set_bkg_data(UINT8 first_tile,
	     UINT8 nb_tiles,
	     unsigned char *data);

void
set_bkg_tiles(UINT8 x,
	      UINT8 y,
	      UINT8 w,
	      UINT8 h,
	      unsigned char *tiles);

void
get_bkg_tiles(UINT8 x,
	      UINT8 y,
	      UINT8 w,
	      UINT8 h,
	      unsigned char *tiles);

void
move_bkg(UINT8 x,
	 UINT8 y);

void
scroll_bkg(INT8 x,
	   INT8 y);

/* ************************************************************ */

void
set_win_data(UINT8 first_tile,
	     UINT8 nb_tiles,
	     unsigned char *data);

void
set_win_tiles(UINT8 x,
	      UINT8 y,
	      UINT8 w,
	      UINT8 h,
	      unsigned char *tiles);

void
get_win_tiles(UINT8 x,
	      UINT8 y,
	      UINT8 w,
	      UINT8 h,
	      unsigned char *tiles);

void
move_win(UINT8 x,
	 UINT8 y);

void
scroll_win(INT8 x,
	   INT8 y);

/* ************************************************************ */

void
set_sprite_data(UINT8 first_tile,
		UINT8 nb_tiles,
		unsigned char *data);

void
get_sprite_data(UINT8 first_tile,
		UINT8 nb_tiles,
		unsigned char *data);

void
set_sprite_tile(UINT8 nb,
		UINT8 tile);

UINT8
get_sprite_tile(UINT8 nb);

void
set_sprite_prop(UINT8 nb,
		UINT8 prop);

UINT8
get_sprite_prop(UINT8 nb);

/** Moves the given sprite to the given position on the
    screen.
    Dont forget that the top left visible pixel on the screen
    is at (8,16).  To put sprite 0 at the top left, use
    move_sprite(0, 8, 16);
*/
void
move_sprite(UINT8 nb,
	    UINT8 x,
	    UINT8 y);

/** Moves the given sprite relative to it's current position.
 */
void
scroll_sprite(INT8 nb,
	      INT8 x,
	      INT8 y);

/* ************************************************************ */

void
set_data(unsigned char *vram_addr,
	 unsigned char *data,
	 UINT16 len);

void
get_data(unsigned char *data,
	 unsigned char *vram_addr,
	 UINT16 len);

void
set_tiles(UINT8 x,
	  UINT8 y,
	  UINT8 w,
	  UINT8 h,
	  unsigned char *vram_addr,
	  unsigned char *tiles);

void
get_tiles(UINT8 x,
	  UINT8 y,
	  UINT8 w,
	  UINT8 h,
	  unsigned char *tiles,
	  unsigned char *vram_addr);

#endif /* _GB_H */
