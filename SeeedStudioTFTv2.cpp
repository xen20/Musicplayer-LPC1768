/* mbed library for resistive touch pads
 * uses 4 pins - 2 IO and 2 Analog

 * c 2011 Peter Drescher - DC2PD
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "mbed.h"
#include "SeeedStudioTFTv2.h"

SeeedStudioTFTv2::SeeedStudioTFTv2(PinName xp, PinName xm, PinName yp, PinName ym,
                                   PinName mosi, PinName miso, PinName sclk,
                                   PinName csTft, PinName dcTft, PinName blTft):

#ifdef USE_SDCARD
    SDFileSystem(mosi,miso,sclk,csSd, "sdc"),
#endif
    SPI_TFT_ILI9341(mosi,miso,sclk,csTft,NC,dcTft, "tft"),
    bl(blTft)
{
#ifndef USE_SDCARD
    // sd card
    //DigitalOut cs(csSd);
    //cs = 1;
#endif
    // backlight
    bl = 1;
    font = NULL;
    // touch screen pins
    _xp = xp;
    _yp = yp;
    _xm = xm;
    _ym = ym;
    // default touch calibration
    // orientation     //      0      1      2      3
    x_off = 108000;  //  17252  16605 108755 108000
    y_off =  22000;  //  22330 105819  97167  22000
    pp_tx =   -291;  //    378    289   -390   -291
    pp_ty =    356;  //    261   -355   -239    356
}

void SeeedStudioTFTv2::setBacklight(bool enabled)
{
    bl = enabled;
}

int SeeedStudioTFTv2::readTouch(PinName p, PinName m, PinName a, PinName i)
{
    DigitalOut _p(p);
    _p = 1;
    DigitalOut _m(m);
    _m = 0;
    AnalogIn   _a(a);
    AnalogIn   _i(i); // this pin has to be high Z (DigitalIn may also work)
    wait_us(10);
    return _a.read_u16();
}

SeeedStudioTFTv2::TOUCH SeeedStudioTFTv2::getTouch(point& p)
{
    int y2 = readTouch(_xp,_xm,_yp,_ym);
    int x2 = readTouch(_yp,_ym,_xp,_xm);
    int y1 = readTouch(_xp,_xm,_yp,_ym);
    int x1 = readTouch(_yp,_ym,_xp,_xm);
    int xd = x1 - x2;
    int yd = y1 - y2;
    xd = (xd > 0) ? xd : -xd;
    yd = (yd > 0) ? xd : -xd;
    p.x = x1 + x2;
    p.y = y1 + y2;
#if 0
    DigitalOut _p(_xp);
    _p = 1;
    DigitalOut _m(_ym);
    _m = 0;
    AnalogIn   _ax(_xm);
    AnalogIn   _ay(_yp);
    wait_us(20);
    int ax = _ax.read_u16();
    int ay = _ay.read_u16();
    float z = 0;
    z  = (float)ay / ax / x / 2 * 0x10000;
#endif
    const int th = 8000;
    const int df =  100;
    TOUCH touch;
    if (x1 < th || x2 < th ||
            y1 < th || y2 < th) {
        p.x = 0;
        p.y = 0;
        touch = NO;
    } else if (xd > df || yd > df) {
        touch = MAYBE;
    } else {
        touch = YES;
    }
    //locate(0,50);
    //printf("x: %6i y: %6i",p.x,p.y);
    return touch;
}

void SeeedStudioTFTv2::calibrate(void)
{
    int i;
    int a = 0,b = 0,c = 0, d = 0;
    int pos_x = 0, pos_y = 0;
    point p;

    cls();
    foreground(White);    // set chars to white
    line(0,3,6,3,White);
    line(3,0,3,6,White);
    if (font)
    {
        // get the center of the screen
        pos_x = columns() / 2 - 3;
        pos_x = pos_x * font[1];
        pos_y = (rows() / 2) - 1;
        pos_y = pos_y * font[2];
        locate(pos_x,pos_y);
        printf("press cross    ");
        locate(pos_x,pos_y + font[2]);
        printf("to calibrate   ");
    }
    for (i=0; i<5; i++) {
        while (getTouch(p) != YES)
            /*nothing*/;
        a += p.x;
        b += p.y;
    }
    a = a / 5;
    b = b / 5;
    if (font)
    {
        locate(pos_x,pos_y);
        printf("ok             ");
        locate(pos_x,pos_y + font[2]);
        printf("release touch  ");
    }
    while (getTouch(p) != NO)
        /*nothing*/;
    cls();
    line(width() -5, height() - 8,width() - 5,height() -1,White);   // paint cross
    line(width() - 8,height() - 5,width() - 1,height() - 5,White);
    if (font)
    {
        locate(pos_x,pos_y);
        printf("press cross    ");
        locate(pos_x,pos_y + font[2]);
        printf("to calibrate   ");
    }
    for (i=0; i<5; i++) {
        while (getTouch(p) != YES)
            /*nothing*/;
        c+= p.x;
        d+= p.y;
    }
    c = c / 5;
    d = d / 5;
    x_off = a;
    y_off = b;
    i = c-a;  // delta x
    pp_tx = i / (width() - 6);
    i = d-b;  // delta y
    pp_ty = i / (height() - 6);
    if (font)
    {
        locate(pos_x,pos_y);
        printf("Calibrated     ");
        locate(pos_x,pos_y + font[2]);
        printf("x %6i %4i", x_off, pp_tx);
        locate(pos_x,pos_y + 2*font[2]);
        printf("y %6i %4i", y_off, pp_ty);
    }
    while (getTouch(p) != NO)
        /*nothing*/;
    cls();
}

point SeeedStudioTFTv2::toPixel(point p)
{
    p.x -= x_off;
    p.x /= pp_tx;
    int w = width();
    if (p.x > w) p.x = w;
    if (p.x < 0) p.x = 0;
    p.y -= y_off;
    p.y /= pp_ty;
    int h = height();
    if (p.y > h) p.y = h;
    if (p.y < 0) p.y = 0;
    return (p);
}

bool SeeedStudioTFTv2::getPixel(point& p)
{
    TOUCH touch = getTouch(p);
    p = toPixel(p);
    return touch == YES;
}

