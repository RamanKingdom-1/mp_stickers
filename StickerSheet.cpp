 #include "StickerSheet.h"
 
 
 StickerSheet::~StickerSheet() {
    delete holder;
 }

  StickerSheet& StickerSheet::operator=(const StickerSheet& other) {
    delete holder;
    holder = new Image(*other.holder);
    location.clear();
    layers_.clear();
    original_picture = *other.holder;

    for (std::vector<int> x : other.location) {
        location.push_back(x);
    }

    for (Image* y: other.layers_) {
        layers_.push_back(y);
    }

    return *this;
  }


  StickerSheet::StickerSheet(const StickerSheet& other) {
    holder = new Image(*other.holder);
    original_picture = *other.holder;
    for (std::vector<int> x : other.location) {
        location.push_back(x);
    }

    for (unsigned i = 0; i < other.layers_.size(); i++) {
        layers_.push_back(&*other.layers_[i]);
    }
  }


  /**
   * Initializes this StickerSheet with a deep copy of the base picture.
   *
   * @param picture The base picture to use in the StickerSheet
   */
  StickerSheet::StickerSheet(Image& picture) {
    holder = new Image(picture);
    original_picture = picture;
    width_in_case_of_negative = 0;
    height_in_case_of_negative = 0;
    width_in_case_of_extra = 0;
    height_in_case_of_extra = 0;
  }

  /**
   * Adds a `sticker` to the StickerSheet, so that the top-left of the sticker's
   * `Image` is at `(x, y)` on the StickerSheet.
   *
   * The sticker must be added to the lowest possible layer available.
   *
   * If all available layers have been filled, a new layer must be added above
   * all existing layers.
   *
   * @param sticker The Image of the sticker.
   * @param x The x location of the sticker on the StickerSheet
   * @param y The y location of the sticker on the StickerSheet
   *
   * @return The zero-based layer index the sticker was added to.
   */
  int StickerSheet::addSticker(Image& sticker, int x, int y) {
    Image* new_sticker = &sticker;
    layers_.push_back(new_sticker);
    std::vector<int> z = {x,y};
    location.push_back(z);
    return layers_.size() - 1;
  }

  /**
   * Adds a `sticker` to the StickerSheet at layer `layer`, so that the top-left
   * of the sticker's `Image` is at `(x, y)` on the StickerSheet. If there is already
   * a sticker at `layer` it should be replaced and if `layer` is beyond the top of the
   * existing layers, add a new layer on top.
   *
   * @param sticker The Image of the sticker.
   * @param layer The layer where the sticker is to be placed.
   * @param x The x location of the sticker on the StickerSheet
   * @param y The y location of the sticker on the StickerSheet
   *
   * @return The zero-based layer index the sticker was added to.
   */
  int StickerSheet::setStickerAtLayer(Image& sticker, unsigned layer, int x, int y) {
    if (layer >= 0 && layer < layers_.size()) {
        Image* new_sticker = &sticker;
        layers_[layer] = new_sticker;
        std::vector<int> z = {x,y};
        location[layer] = z;
        return layer;
    } else {
        return addSticker(sticker,x,y);
    }
  }

  /**
   * Changes the x and y coordinates of the Image in the specified layer.
   *
   * If the layer is invalid or does not contain a sticker, this function
   * must return false. Otherwise, this function returns true.
   *
   * @param index Zero-based layer index of the sticker.
   * @param x The new x location of the sticker on the StickerSheet
   * @param y The new y location of the sticker on the StickerSheet
   *
   * @return `true` if the translate was successful; otherwise `false`.
   */
  bool StickerSheet::translate(unsigned index, int x, int y) {
    if (index >= 0 && index < layers_.size()) {
        location[index][0] = x;
        location[index][1] = y;
        return true;
    } else {
        return false;
    }
  }

  /**
   * Removes the sticker at the given zero-based layer index. Make sure that the
   * other stickers don't change order.
   *
   * @param index The layer in which to delete the png
   */
  void StickerSheet::removeSticker(unsigned index) {
    copy_original_picture();
    if (index >= 0 && index < layers_.size()) {
        location.erase(location.begin() + index);
        layers_.erase(layers_.begin() + index);
    }
  }

  void StickerSheet::copy_original_picture() {
    holder -> resize(original_picture.width(), original_picture.height());
        for (unsigned x = 0; x < original_picture.height(); x++) {
            for (unsigned y = 0; y < original_picture.width(); y++) {
                cs225::HSLAPixel& helper = holder -> getPixel(y,x);
                cs225::HSLAPixel& helper_second = original_picture.getPixel(y,x);
                helper = helper_second;
            }
        }
        width_in_case_of_extra = 0;
        width_in_case_of_negative = 0;
        height_in_case_of_extra = 0;
        height_in_case_of_negative = 0;
  }

  /**
   * Returns a pointer to the sticker at the specified index,
   * not a copy of it. That way, the user can modify the Image.
   *
   * If the index is invalid, return NULL.
   *
   * @param index The layer in which to get the sticker.
   *
   * @return A pointer to a specific Image in the StickerSheet
   */
  Image* StickerSheet::getSticker(unsigned index) {
    if (index >= 0 && index < layers_.size()) {
        return layers_[index];
    } else {
        return NULL;
    }


  }

  /**
   * Returns the total number of layers available on the Stickersheet.
   *
   * @return The total number of layers
   */
  int StickerSheet::layers() const {
    return layers_.size();
  }

  /**
   * Renders the whole StickerSheet on one Image and returns that Image.
   *
   * The base picture is drawn first and then each sticker is drawn in order
   * starting with layer zero (0), then layer one (1), and so on.
   *
   * If a pixel's alpha channel in a sticker is zero (0), no pixel is drawn
   * for that sticker at that pixel. If the alpha channel is non-zero, a
   * pixel is drawn. (Alpha blending is awesome, but not required.)
   *
   * The returned image always includes the full contents of the picture and
   * all stickers. It should expand in each corresponding direction if
   * stickers go beyond the edge of the picture.
   *
   * @return An Image object representing the drawn scene
   */
  Image StickerSheet::render() const {
    for (unsigned i = 0; i < layers_.size(); i++) {
        if (static_cast<int>(location[i][0] + layers_[i] -> width()) > static_cast<int> (holder -> width())) {
            if (static_cast<int>(location[i][0] + layers_[i] -> width()) > width_in_case_of_extra) {
                width_in_case_of_extra = location[i][0] + layers_[i] -> width();
                positive_values(*layers_[i], location[i][0], location[i][1]);
            }
        }

        if (location[i][0] < 0){
            if (location[i][0] * -1 > width_in_case_of_negative) {
                width_in_case_of_negative = location[i][0];                
                negative_values(*layers_[i], width_in_case_of_negative, holder -> height());
                width_in_case_of_negative *= -1;
            }
        }

        if (static_cast<int>(location[i][1] + layers_[i] -> height()) > static_cast<int>(holder -> height())) {
            if (static_cast<int>(location[i][1] + layers_[i] -> height()) > height_in_case_of_extra) {
                height_in_case_of_extra = location[i][1] + layers_[i] -> height();
                positive_values(*layers_[i], location[i][0], location[i][1]);
            }
        }

        if (location[i][1] < 0){
            if (location[i][1] * -1 > width_in_case_of_negative) {
                height_in_case_of_negative = location[i][1];
                negative_values(*layers_[i], holder -> width(), height_in_case_of_negative);
                height_in_case_of_negative *= -1;
            }
        }
              
        render_helper(*layers_[i], location[i][0], location[i][1]);
    }

    return *holder;
  }
  

  void StickerSheet::negative_values(Image& currently,int width_, int height_)const {
   
   if (width_ < 0) {
        Image temporary  = Image((width_ * -1) + holder -> width(), holder -> height());
        for (unsigned i = 0; i < holder -> height(); i++) {
            for (unsigned p = 0; p < holder -> width(); p++) {
                cs225::HSLAPixel helper = holder -> getPixel(p,i);
                cs225::HSLAPixel& helper_main = temporary.getPixel(p + (width_ * -1),i);
                helper_main = helper;
            
            }
        }
        holder -> resize(holder -> width() +  (width_ * -1), holder -> height());
        for (unsigned x = 0; x < temporary.height(); x++) {
            for (unsigned y = 0; y < temporary.width(); y++) {
                cs225::HSLAPixel& helper = holder -> getPixel(y,x);
                cs225::HSLAPixel& helper_second = temporary.getPixel(y,x);
                helper = helper_second;
            }
        }

    } else {
        Image temporary  = Image(holder -> width(), holder -> height()+ (height_ * -1));
         for (unsigned i = 0; i < holder -> height(); i++) {
            for (unsigned p = 0; p < holder -> width(); p++) {
                cs225::HSLAPixel helper = holder -> getPixel(p,i);
                cs225::HSLAPixel& helper_main = temporary.getPixel(p,i+ (height_ * -1));
                helper_main = helper;
            
            }
        }
        holder -> resize(holder -> width(), holder -> height() + (height_ * -1));
        for (unsigned x = 0; x < temporary.height(); x++) {
            for (unsigned y = 0; y < temporary.width(); y++) {
                cs225::HSLAPixel& helper = holder -> getPixel(y,x);
                cs225::HSLAPixel& helper_second = temporary.getPixel(y,x);
                helper = helper_second;
            }
        }

    }

  }

  void StickerSheet::positive_values(Image& currently,int width_, int height_)const {
      if (static_cast<int>(width_ + currently.width()) > static_cast<int>(holder -> width())) {
        Image temporary  = Image(width_ + currently.width(), holder -> height());
        for (unsigned i = 0; i < holder -> height(); i++) {
            for (unsigned p = 0; p < holder -> width(); p++) {
                cs225::HSLAPixel helper = holder -> getPixel(p,i);
                cs225::HSLAPixel& helper_main = temporary.getPixel(p,i);
                helper_main = helper;
            
            }
        }
        holder -> resize(width_ + currently.width(), holder -> height());
        for (unsigned x = 0; x < temporary.height(); x++) {
            for (unsigned y = 0; y < temporary.width(); y++) {
                cs225::HSLAPixel& helper = holder -> getPixel(y,x);
                cs225::HSLAPixel& helper_second = temporary.getPixel(y,x);
                helper = helper_second;
            }
        }
      } else {
        Image temporary  = Image(holder -> width(), height_ + currently.height());
        for (unsigned i = 0; i < holder -> height(); i++) {
            for (unsigned p = 0; p < holder -> width(); p++) {
                cs225::HSLAPixel helper = holder -> getPixel(p,i);
                cs225::HSLAPixel& helper_main = temporary.getPixel(p,i);
                helper_main = helper;
            
            }
        }
        holder -> resize(holder -> width(), height_ + currently.height());
        for (unsigned x = 0; x < temporary.height(); x++) {
            for (unsigned y = 0; y < temporary.width(); y++) {
                cs225::HSLAPixel& helper = holder -> getPixel(y,x);
                cs225::HSLAPixel& helper_second = temporary.getPixel(y,x);
                helper = helper_second;
            }
        }
      }
  }

  void StickerSheet::render_helper(Image& currently,int x, int y)const {
    if (height_in_case_of_negative > static_cast<int>(holder -> height()) || height_in_case_of_negative < 0) {
        height_in_case_of_negative = 0;
    }

    if (width_in_case_of_negative >static_cast<int>(holder -> width()) || width_in_case_of_negative < 0) {
        width_in_case_of_negative = 0;
    }
    
    
    int z = y + height_in_case_of_negative;
    int u = x + width_in_case_of_negative;
    int width_actual = 0;
    int height_actual = 0;
    


    for (int i = z; i < static_cast<int>(currently.height()) + z; i++) {
      for (int p = u; p < static_cast<int>(currently.width()) + u; p++) {
        
          cs225::HSLAPixel& helper = holder -> getPixel(p,i);
          cs225::HSLAPixel& helper_main = currently.getPixel(width_actual,height_actual);
          if (helper_main.a != 0) {
            helper = helper_main;
          }
          width_actual++;
      }
      height_actual++;
      width_actual = 0;
    }
  }
  