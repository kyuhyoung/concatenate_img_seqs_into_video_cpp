# concatenate_img_seqs_into_video_cpp
Concatenate images from multiple directories into a image sequence (and then optionally into mp4 or animated webp).
## What for

![image](https://user-images.githubusercontent.com/12492992/118100712-ba6bc200-b411-11eb-932a-d5080f962880.png)

## Required
  ### OpenCV
  ### webp (img2webp)
  ```console
apt install webp
  ```
## Usage  
### format
 ```console
./concatenate_img_seqs_into_video_cpp param1 param2 param3 param4 param5 param5 param6 param7 param8 .............. paramN
  ```
#### param1 : [integer] Save as video if negative. Save as webp if positive. Otherwise just save as image sequence.
#### param2 : [string] Output directory.
#### param3 : [integer] Concatenate vertically if positive. Otherwise horisontally.
#### param4 : [integer] Align left for vertical concatenation if negative. Align center for vertical concatenation if zero.  Align right for vertical concatenation if positive.
Align top for horizontal concatenation if negative. Align center for horizontal concatenation if zero.  Align bottom for horizontal concatenation if positive.
#### param5 : [float] Between 0 ~ 1. Factor of margin between concatenated images.
#### param6 : [integer] Between 0 ~ 255. Gray pixel value of margin area.
#### param7 : [integer] Start frame index if positive. Otherwise automatically.
#### param8 : [integer] End frame index if positive and larger than <param7>. Otherwise automatically.
#### param9 : [string] Directory of 1st image sequence to be concatenated.
#### param10 : [string] Directory of 2nd image sequence to be concatenated.
#### param11 : [string] Directory of 3rd image sequence to be concatenated. 
#### param8+N : [string] Directory of N th image sequence to be concatenated.

### Example of vertically and right-aligned concatenating images under the five directories "../seq1", "../seq2", "../seq3", "../seq4" and "../seq5" into a video which is saved under "../output".  The length of margin between images is "1%" of total height and area is colored as gray value of "128".  The start frame index is "0" and end frame index is "4500". 
 ```console
./concatenate_img_seqs_into_video_cpp -1 ../output/ 1 1 0.01 128 0 4500 ../seq1 ../seq2 ../seq3 ../seq4 ../seq5
  ```
