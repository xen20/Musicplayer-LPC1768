/* mbed library for touchscreen connected to 4 mbed pins
 * derive from SPI_TFT lib
 * Copyright (c) 2011 Peter Drescher - DC2PD
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef MBED_TOUCH_H
#define MBED_TOUCH_H

#include "mbed.h"
#include "SPI_TFT_ILI9341.h"
#ifdef USE_SDCARD
#include "SDFileSystem.h" // import the SDFileSystem library 
#endif

struct point {
    int x;
    int y;
};

class SeeedStudioTFTv2 : public  
#ifdef USE_SDCARD
    SDFileSystem,
#endif
    SPI_TFT_ILI9341
{
public:
    /** create a TFT with touch object connected to the pins:
     *
     * @param pin xp resistiv touch x+
     * @param pin xm resistiv touch x-
     * @param pin yp resistiv touch y+
     * @param pin ym resistiv touch y-
     * @param mosi,miso,sclk SPI connection to TFT
     * @param cs pin connected to CS of display
     * @param reset pin connected to RESET of display
     * based on my SPI_TFT lib
     */
    SeeedStudioTFTv2(PinName xp, PinName xm, PinName yp, PinName ym,
                     PinName mosi, PinName miso, PinName sclk,
                     PinName csTft, PinName dcTft, PinName blTft);

    typedef enum { YES, MAYBE, NO } TOUCH;
    TOUCH getTouch(point& p);


    void setBacklight(bool enabled);
    
    /** calibrate the touch display
     *
     * User is asked to touch on two points on the screen
     */
    void calibrate(void);

    /** read x and y coord on screen
     *
     * @returns point(x,y)
     */
    bool
    getPixel(point& p);

    /** calculate coord on screen
    *
    * @param a_point point(analog x, analog y)
    * @returns point(pixel x, pixel y)
    *
    */
    point toPixel(point p);

protected:
    PinName _xm;
    PinName _ym;
    PinName _xp;
    PinName _yp;
    DigitalOut bl;

    //typedef enum { YES, MAYBE, NO } TOUCH;
    //TOUCH getTouch(point& p);
    int readTouch(PinName p, PinName m, PinName a, PinName i);

    int x_off,y_off;
    int pp_tx,pp_ty;
};

#endif

