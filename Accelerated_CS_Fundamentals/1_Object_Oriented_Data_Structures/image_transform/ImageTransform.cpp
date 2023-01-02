#include <cmath>
#include <cstdlib>
#include <iostream>

#include "ImageTransform.h"
#include "uiuc/HSLAPixel.h"
#include "uiuc/PNG.h"

/* ******************
(Begin multi-line comment...)

Write your name and email address in the comment space here:

Name:
Email:

(...end multi-line comment.)
******************** */

using uiuc::HSLAPixel;
using uiuc::PNG;

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
PNG grayscale(PNG image)
{
  /// This function is already written for you so you can see how to
  /// interact with our PNG class.
  for (unsigned x = 0; x < image.width(); x++)
  {
    for (unsigned y = 0; y < image.height(); y++)
    {
      HSLAPixel &pixel = image.getPixel(x, y);

      // `pixel` is a reference to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly. No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel.s = 0;
    }
  }

  return image;
}

/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel euclidean
 * distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 160 pixels away, the luminance will always decreased by 80%.
 *
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
PNG createSpotlight(PNG image, int centerX, int centerY)
{
  unsigned height = image.height();
  unsigned width = image.width();

  for (unsigned i = 0; i < width; i++)
  {
    for (unsigned j = 0; j < height; j++)
    {
      HSLAPixel &pixel = image.getPixel(i, j);
      double &lumi = pixel.l;

      double dist = sqrt((centerX - i) * (centerX - i) + (centerY - j) * (centerY - j));

      if (dist >= 160)
        lumi -= lumi * 0.8;
      else
        lumi -= lumi * (dist * 0.5) * 0.01;
    }
  }
  return image;
}

/**
 * Returns a image transformed to Illini colors.
 *
 * The hue of every pixel is set to the a hue value of either orange or
 * blue, based on if the pixel's hue value is closer to orange than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The illinify'd image.
 **/
PNG illinify(PNG image)
{
  /*Illini Orange (11) or Illini Blue (216)*/

  unsigned height = image.height();
  unsigned width = image.width();

  for (unsigned i = 0; i < width; i++)
  {
    for (unsigned j = 0; j < height; j++)
    {
      HSLAPixel &pixel = image.getPixel(i, j);
      double &hue = pixel.h;

      int illini_orange;
      if (hue > 11.0)
      {
        illini_orange = min(hue - 11.0, 360.0 - hue + 11.0);
      }
      else
      {
        illini_orange = min(11.0 - hue, 360.0 - 11.0 + hue);
      }

      int illini_blue;
      if (hue > 216.0)
      {
        illini_blue = min(hue - 216.0, 360.0 - hue + 216.0);
      }
      else
      {
        illini_blue = min(216.0 - hue, 360.0 - 216.0 + hue);
      }

      if (illini_orange < illini_blue)
        hue = 11.0;
      else
        hue = 216.0;
    }
  }

  return image;
}

/**
 * Returns an immge that has been watermarked by another image.
 *
 * The luminance of every pixel of the second image is checked, if that
 * pixel's luminance is 1 (100%), then the pixel at the same location on
 * the first image has its luminance increased by 0.2.
 *
 * @param firstImage  The first of the two PNGs, which is the base image.
 * @param secondImage The second of the two PNGs, which acts as the stencil.
 *
 * @return The watermarked image.
 */
PNG watermark(PNG firstImage, PNG secondImage)
{
  unsigned min_height = min(firstImage.height(), secondImage.height());
  unsigned min_width = min(firstImage.width(), secondImage.width());

  for (unsigned i = 0; i < min_width; i++)
  {
    for (unsigned j = 0; j < min_height; j++)
    {
      HSLAPixel &first_pixel = firstImage.getPixel(i, j);
      double &first_lumi = first_pixel.l;

      HSLAPixel &second_pixel = secondImage.getPixel(i, j);
      double &second_lumi = second_pixel.l;

      if (second_lumi == 1.0)
        first_lumi = min(1.0, first_lumi + 0.2);
    }
  }

  return firstImage;
}
