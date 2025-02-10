#include "Image.h"


   





    /**
     * Lighten an Image by increasing the luminance of every pixel by 0.1.
     * This function ensures that the luminance remains in the range [0, 1].
    **/

    void Image::lighten() {
        for (unsigned x = 0; x < height(); x++) {
            for (unsigned y = 0; y < width(); y++) {
                cs225::HSLAPixel& helper = getPixel(y,x);
                if (helper.l + 0.1 < 1 && helper.l + 0.1 > 0) {
                    helper.l = helper.l + 0.1;
                } else {
                    helper.l = 1;
                }
            }
        } 
    }

    /**
     * Lighten an Image by increasing the luminance of every pixel by `amount`.
     * This function ensures that the luminance remains in the range [0, 1].
     *
     * @param amount The desired increase in luminance.
    **/
    void Image::lighten(double amount) {
        for (unsigned x = 0; x < height(); x++) {
            for (unsigned y = 0; y < width(); y++) {
                cs225::HSLAPixel& helper = getPixel(y,x);
                if (helper.l + amount < 1 && helper.l + amount > 0) {
                    helper.l = helper.l + amount;
                } else {
                    helper.l = 1;
                }
            }
        } 
    }

    /**
     * Darken an Image by decreasing the luminance of every pixel by 0.1.
     * This function ensures that the luminance remains in the range [0, 1].
    **/
    void Image::darken() {
        for (unsigned x = 0; x < height(); x++) {
            for (unsigned y = 0; y < width(); y++) {
                cs225::HSLAPixel& helper = getPixel(y,x);
                if (helper.l - 0.1 < 1 && helper.l - 0.1 > 0) {
                    helper.l = helper.l - 0.1;
                } else {
                    helper.l = 0;
                }
            }
        } 
    }

    /**
     * Darkens an Image by decreasing the luminance of every pixel by `amount`.
     * This function ensures that the luminance remains in the range [0, 1].
     *
     * @param amount The desired decrease in luminance.
    **/
    void Image::darken(double amount) {
        for (unsigned x = 0; x < height(); x++) {
            for (unsigned y = 0; y < width(); y++) {
                cs225::HSLAPixel& helper = getPixel(y,x);
                if (helper.l - amount < 1 && helper.l - amount > 0) {
                    helper.l = helper.l - amount;
                } else {
                    helper.l = 0;
                }
            }
        } 
    }


    /**
     * Saturates an Image by increasing the saturation of every pixel by 0.1.
     * This function ensures that the saturation remains in the range [0, 1].
    **/
    void Image::saturate() {
        for (unsigned x = 0; x < height(); x++) {
            for (unsigned y = 0; y < width(); y++) {
                cs225::HSLAPixel& helper = getPixel(y,x);
                if (helper.s + 0.1 < 1 && helper.s + 0.1 > 0) {
                    helper.s = helper.s + 0.1;
                }
            }
        } 
    }

    /**
     * Saturates an Image by increasing the saturation of every pixel by `amount`.
     * This function ensures that the saturation remains in the range [0, 1].
     *
     * @param amount The desired increase in saturation.
    **/
    void Image::saturate(double amount) {
        for (unsigned x = 0; x < height(); x++) {
            for (unsigned y = 0; y < width(); y++) {
                cs225::HSLAPixel& helper = getPixel(y,x);
                if (helper.s + amount < 1 && helper.s + amount > 0) {
                    helper.s = helper.s + amount;
                }
            }
        } 
    }

    /**
     * Desaturates an Image by decreasing the saturation of every pixel by 0.1.
     * This function ensures that the saturation remains in the range [0, 1].
    **/
    void Image::desaturate() {
        for (unsigned x = 0; x < height(); x++) {
            for (unsigned y = 0; y < width(); y++) {
                cs225::HSLAPixel& helper = getPixel(y,x);
                if (helper.s - 0.1 < 1 && helper.s - 0.1 > 0) {
                    helper.s = helper.s - 0.1;
                }
            }
        } 
    }

    /**
     * Desaturates an Image by decreasing the saturation of every pixel by `amount`.
     * This function ensures that the saturation remains in the range [0, 1].
     *
     * @param amount The desired decrease in saturation.
    **/
    void Image::desaturate(double amount) {
        for (unsigned x = 0; x < height(); x++) {
            for (unsigned y = 0; y < width(); y++) {
                cs225::HSLAPixel& helper = getPixel(y,x);
                if (helper.s - amount < 1 && helper.s - amount > 0) {
                    helper.s = helper.s - amount;
                }
            }
        } 
    }

    /**
     * Turns the image grayscale.
    **/
    void Image::grayscale() {
        for (unsigned x = 0; x < height(); x++) {
            for (unsigned y = 0; y < width(); y++) {
                cs225::HSLAPixel& helper = getPixel(y,x);
                helper.s = 0;
            }
        } 
    }


    /**
     * Rotates the color wheel by `degrees`. Rotating in a positive direction increases the degree of the hue.
     * This function ensures that the hue remains in the range [0, 360].
     *
     * @param degrees The desired amount of rotation.
    **/
    void Image::rotateColor(double degrees) {
        for (unsigned x = 0; x < height(); x++) {
            for (unsigned y = 0; y < width(); y++) {
                cs225::HSLAPixel& helper = getPixel(y,x);
                double temp = helper.h + degrees;
                while (temp > 360) {
                    temp -= 360;
                }

                while (temp < 0) {
                    temp += 360;
                }
                helper.h = temp;                
            }
        }   
    }

    /**
     * Illinify the image.
    **/
    void Image::illinify() {
        for (unsigned x = 0; x < height(); x++) {
            for (unsigned y = 0; y < width(); y++) {
                cs225::HSLAPixel& helper = getPixel(y,x);
                if(helper.h < 113.5) {
                    helper.h = 11;
                } else {
                    helper.h = 216;
                }
            }
        }
    }


    /**
     * Scale the Image by a given `factor`.
     *
     * For example:
     * - A `factor` of 1.0 does not change the image.
     * - A `factor` of 0.5 results in an image with half the width() and half the height.
     * - A `factor` of 2 results in an image with twice the width() and twice the height.
     *
     * This function both resizes the Image and scales the contents.
     *
     * @param factor Scale factor.
    **/
    void Image::scale(double factor) {
        double temp = factor;
        cs225::PNG temporary = cs225::PNG(width() * temp * 2, height() * temp * 2);
        
        if (temp < 1) {
            temp = 1/temp;
            temp = static_cast<int>(temp);
        
            int width_actual = 0;
            int height_actual = 0;
            for (unsigned i = 0; i < height(); i += temp) {
                for (unsigned p = 0; p < width(); p+= temp) {
                        cs225::HSLAPixel helper = getPixel(p,i);
                        cs225::HSLAPixel& helper_main = temporary.getPixel(width_actual,height_actual);
                        helper_main = helper;
                        width_actual++;
                }
                height_actual++;
                width_actual = 0;
            }
            resize(width() * factor, height() * factor);

            for (unsigned i = 0; i < height();i++) {
                for (unsigned p = 0; p < width(); p++) {
                    if (i < height() && p < width()) {
                    cs225::HSLAPixel& helper = getPixel(p,i);
                    cs225::HSLAPixel& helper_main = temporary.getPixel(p,i);
                    helper = helper_main;
                    }
                }
            }
        } else {
            int width_actual = 0;
            int height_actual = 0;
            for (unsigned i = 0; i < height(); i++) {
                for (int q = 0; q < temp; q++) {
                    for (unsigned p = 0; p < width(); p++) {
                        cs225::HSLAPixel helper = getPixel(p,i);
                        for (int x = 0; x < temp; x++) {
                                cs225::HSLAPixel& helper_main = temporary.getPixel(width_actual,height_actual);
                                helper_main = helper;
                                width_actual++;
                            }
                        
                    }
                    height_actual++;
                    width_actual = 0;
                }
            }
    
            resize(width() * temp, height() * temp);

            for (unsigned i = 0; i < height();i++) {
                for (unsigned p = 0; p < width(); p++) {
                    cs225::HSLAPixel& helper = getPixel(p,i);
                    cs225::HSLAPixel& helper_main = temporary.getPixel(p,i);
                    helper = helper_main;
                }
            }
        }


      


    }

    /**
     * Scales the image to fit within the size (`w` x `h`).  This function preserves
     * the aspect ratio of the image, so the result will always be an image of width()
     * `w` or of height `h` (not necessarily both).
     *
     * For example, if the Image is currently (10, 20) and we want to scale to
     * (100, 100), then this should scale the Image to (50, 100). Note that scaling
     * the image to (100, 200) does not fit within the (100,100) dimensions given
     * and is therefore incorrect.
     *
     * This function both resizes the Image and scales the contents.
     *
     * @param w Desired width() of the scaled Image
     * @param h Desired height of the scaled Image
    **/
    void Image::scale(unsigned w, unsigned h) {
        double factor_w = static_cast<double>(w)/static_cast<double>(width());
        double factor_h = static_cast<double>(h)/static_cast<double>(height());

        if (factor_w < factor_h) {
            scale(factor_w);
        } else {
            scale(factor_h);
        }
    }

    