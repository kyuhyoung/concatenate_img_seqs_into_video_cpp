#include "opencv2/opencv.hpp"
#include <sys/stat.h>
#include <stdio.h>
#include <numeric>
#include <vector>
//#include "gif.h"
using namespace cv;
using namespace std;




//------------ It is the counterpart of basename() in Python. --------------
//	cout << get_exa_exact_file_name_from_path("/home/someuser/somefolder/somefile.someext") << endl;
//	=> somefile
string get_exact_file_name_from_path(const string& str_path)
{
	string filename = str_path;
	// Remove directory if present.
	// Do this before extension removal incase directory has a period character.
	const size_t last_slash_idx = filename.find_last_of("\\/");
	if (std::string::npos != last_slash_idx) filename.erase(0, last_slash_idx + 1);
	// Remove extension if present.
	const size_t period_idx = filename.rfind('.');
	if (std::string::npos != period_idx) filename.erase(period_idx);
	return filename;
}



//------------ c++ counterpart of python function "join" -------------- 
//	experimental::filesystem linker error 시
//	Makefile에 -lstdc++fs 옵션을 주어야 함.
//	https://stackoverflow.com/questions/33149878/experimentalfilesystem-linker-error
//
//	cout << python_join_equivalent("/home/someuser/", "/somefolder/somefile.somext");
//	=> /home/someuser/somefolder/somefile.somext

#include <experimental/filesystem>
string python_join_equivalent(const string& dir_to_file, const string& filename)
{
  std::experimental::filesystem::path dir(dir_to_file);
  std::experimental::filesystem::path fn(filename);    
  std::experimental::filesystem::path full_path = dir / fn;
  return full_path.u8string();                                                                               
}




//-----------------------------------------------------------------------------------------------------  
//	Mat im = imread("/home/folder/im_1280_720.bmp");
//	=> The size of im is (1280, 720)
//	Mat im_resized_1 = resize_image(im, 640, 480, 0, 0, 0);
//	=> The size of im_resized_1 is (640, 480)
//	Mat im_resized_2 = resize_image(im, 0, 0, 0.5, 0.25, 0);
//	=> The size of im_resized_2 is (640, 180)
//	Mat im_resized_3 = resize_image(im, 0, 0, 0, 0, 320);
//	=> The size of im_resized_3 is (320, 180)
Mat resize_image(const Mat& im_ori, int w_new, int h_new, float fx, float fy, int max_side)
{
	Mat im_resized;
	int max_wh = MAX(im_ori.cols, im_ori.rows);
	if(w_new > 0 && h_new > 0) resize(im_ori, im_resized, Size(w_new, h_new), 0, 0);
	else if(fx > 0 && fy > 0) resize(im_ori, im_resized, Size(0, 0), fx, fy);
	else if(max_side < max_wh)
	{
		float ratio = float(max_side) / float(max_wh);
		resize(im_ori, im_resized, Size(0, 0), ratio, ratio);
	}
	else if(max_side > 0)
	{
		im_resized = im_ori.clone();
		cout << "Resize is NOT applied since does NOT have to !!" << endl;
	}
	else { cout << "Can NOT resize image since NO guide is given !!" << endl; exit(0); };
	return im_resized;
}


//-----------------------------------------------------------------------------------------------------  
//	VideoWriter vw = init_video_writer("/home/folder/aaa.avi", 30, Size(640, 480));

VideoWriter init_video_writer(const string& fn_path, double fps, const Size& saiz)
{
	VideoWriter writer;
	writer.open(fn_path, VideoWriter::fourcc('X', 'V', 'I', 'D'), fps, saiz, true);
	if (!writer.isOpened())
	{
		cout << "동영상을 저장하기 위한 초기화 작업 중 에러 발생" << endl;	exit(0);
	}
	return writer;
}




//------------ From a string extract the last number which occurs in that string. --------------
//	cout << get_last_integer_substring("my234st8_abc56.bmp") << endl;
//	=> 56
int get_last_integer_substring(const string& str_ori)
{
	string str = str_ori;
	// For atoi, the input string has to start with a digit, so lets search for the first digit
 	size_t i = str.length() - 1;
    //cout << "str_ori : " << str_ori << endl;    //exit(0);
	int idx_first = -1, idx_last = -1;
	bool last_found = false;
	for (; i >= 0; i--)
	{
		if(last_found)
		{
			if(!isdigit(str[i])) break;
			idx_first = i;
		}
		else
		{
			if(isdigit(str[i]))
			{
				last_found = true;
				idx_last = i;
				idx_first = i;
			}
		}
	}
	// remove the first chars, which aren't digits
	str = str.substr(idx_first, idx_last - idx_first + 1);
    //cout << "str : " << str << endl;    //exit(0);
	// convert the remaining text to an integer
    //cout << "stoi(str) : " << stoi(str) << endl;    //exit(0);
	return stoi(str);
}




//------------ get_id_of_file_as_number --------------
//	cout << get_last_integer_substring("my_image_000023.bmp") << endl;
//	=> 23
int get_id_of_file_as_number(const string& fn)
{
	string fn_wo_ext = get_exact_file_name_from_path(fn);
    //cout << "fn_wo_ext : " << fn_wo_ext << endl;    exit(0);
	return get_last_integer_substring(fn_wo_ext);
}

//------------ Make a string lowercase --------------  
//	cout << make_string_lowercase("abcDEfGhijK") << endl;
//	=> abcdefghijk

#include <algorithm>
#include <string>
string make_string_lowercase(string input)
{
	transform(input.begin(), input.end(), input.begin(), ::tolower);
 	return input;
}

//------------ Check if a file is a image. -------------- 
//	cout << is_image_file("/home/someuser/flower.png") << endl;
//	=> true
//	cout << is_image_file("/home/someuser/poet.txt") << endl;
//	=> false

bool is_image_file(const string& filename)
{
	size_t pos = filename.rfind('.');
 	if (pos == string::npos) return false;
 	string ext = make_string_lowercase(filename.substr(pos + 1));
 	if (ext == "jpg" || ext == "jpeg" || ext == "gif" || ext == "bmp" || ext == "png" ||
 		ext == "pgm" || ext == "tif" || ext == "ppm" || ext == "tiff" || ext == "pnm")
 			return true;
 	return false;
 }




//-----------------------------------------------------------------------------------------------------  
//	vector<int> li_idx({ 1, 3, 2 });	
//	vector<vector<int> li_li_id;
//	vector<int> li_id_1({ 3, 5, 7, 9, 11 });	
//	vector<int> li_id_1({ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });	
//	vector<int> li_id_3({ 1, 4, 5, 8, 9, 11, 13 });
//	li_li_id.push_back(li_id_1);	li_li_id.push_back(li_id_2);	li_li_id.push_back(li_id_3);
//	get_next_index(li_idx, li_li_id);
//	=> li_idx becomes from { 1, 3, 2 } to { 3, 7, 4 } which are indices of the common element "9"
//	get_next_index(li_idx, li_li_id);
//	=> li_idx becomes from { 3, 7, 4 } to { 4, 9, 5 } which are index of the common element "11"

bool get_next_index(vector<int>& li_idx, const vector<vector<int>>& li_li_id)
{
	bool is_the_end = false;
	int is_max = -1, id_max = -1000000, n_seq = li_idx.size();
	while(!is_the_end)
	{
		int id_min = 1000000;
		for(int iS = 0; iS < n_seq; iS++)
		{
			if(iS == is_max) continue;
			int idx_cur = li_idx[iS] + 1, n_elem = li_li_id[iS].size();
			if(idx_cur == n_elem)
			{
				is_the_end = true;  break;
			}
			li_idx[iS] = idx_cur;
			int id_cur = li_li_id[iS][idx_cur];
			if(id_cur < id_min) id_min = id_cur;
			if(id_cur > id_max)
			{
				id_max = id_cur;
				is_max = iS;
			}
		}
		if(is_the_end) break;
		//cout << "id_max : " << id_max << ", id_min : " << id_min << endl;
		if(id_max == id_min) break;
	}
	//cout << "is_the_end : " << is_the_end << endl;  //exit(0);
	return !is_the_end;
}
	




int mkdirs(const char *path, mode_t mode = 0733)	// another candidate : 0766  
{ 
	char tmp_path[2048]; 
	const char *tmp = path; 
	int len = 0, ret; 
	if(path == NULL || strlen(path) >= 2048) return -1; 
	while((tmp = strchr(tmp, '/')) != NULL) 
	{ 
		len = tmp - path; 
		tmp++; 
		if(len == 0) continue; 
		strncpy(tmp_path, path, len); 
		tmp_path[len] = 0x00; 
		if((ret = mkdir(tmp_path, mode)) == -1) 
		{ 
			if(errno != EEXIST) return -1; 
		} 
	} 
	return mkdir(path, mode); 
}




//-----------------------------------------------------------------------------------------------------  
//	Mat im = imread("aaa.bmp");
//	save_one_image_under_directory(im, "/home/folder", "bbb.bmp");
//	=> An image is saved at /home/folder/bbb.bmp".

void save_one_image_under_directory(const Mat& im, const string& dir_save, const string& fn_img)
{
	//mkdir_if_not_exist(dir_save.c_str());
	mkdirs(dir_save.c_str());
	string path_res = python_join_equivalent(dir_save, fn_img);
    //cout << "path_res : " << path_res << endl;  //exit()
    //cout << "im.size() : " << im.size() << endl;    exit(0);
	imwrite(path_res, im);
}



//-----------------------------------------------------------------------------------------------------  
//	VideoWriter vw;
//	Mat im = imread("/home/folder/bbb.bmp");
//	vw = write_one_frame_to_video(vw, im, true, "/home/folder/aaa.avi");
//	im = imread("/home/folder/ccc.bmp");
//	vw = write_one_frame_to_video(vw, im, false, "/home/folder/aaa.avi");
//	vw.realese();
//	=> A video is saved at "/home/folder/aaa.avi" whose first frame is "/home/folder/bbb.bmp" and the second (last) frame is "/home/folder/ccc.bmp".

VideoWriter write_one_frame_to_video(VideoWriter& vw, Mat& im, bool is_first_frame, const string& path_vid,          double fps, int max_side)
{
	if(max_side > 0) im = resize_image(im, 0, 0, 0, 0, max_side);
	if(is_first_frame) vw = init_video_writer(path_vid, fps, im.size());
	vw.write(im);
	return vw;
}






//---------------------------------------------------------------------
//	vector<vector<string>> li_li_path;
//	vector<string> li_path_1({ "/home/dir1/001.bmp", "/home/dir1/002.bmp", "/home/dir1/003.bmp" });	
//	vector<string> li_path_2({ "/home/dir2/000.bmp", "/home/dir2/002.bmp", "/home/dir2/003.bmp" });	
//	vector<string> li_path_3({ "/home/dir3/002.bmp", "/home/dir3/003.bmp", "/home/dir3/004.bmp" });
//	li_li_path.push_back(li_path_1);	li_li_path.push_back(li_path_2);	li_li_path.push_back(li_path_3);
//	vector<vector<int>> li_li_id = get_list_of_list_of_ids_as_number(li_li_path);
//	for(int i1 = 0; i1 < li_li_id.size(); i1++) 
//	{ 
//		for(int i2 = 0; i2 < li_li_id[i1].size(); i2++) cout << li_li_id[i1][i2] << ", ";
//		cout << endl; 
//	}
//	=> 1, 2, 3 
//	=> 0, 2, 3 
//	=> 2, 3, 4 

vector< vector<int> > get_list_of_list_of_ids_as_number(const vector< vector<string> >& li_li_path)
{
	vector< vector<int> > li_li_id;
 	for(int iL = 0; iL < li_li_path.size(); iL++)
 	{
 		vector<int> li_id;
 		for(int iP = 0; iP < li_li_path[iL].size(); iP++) li_id.push_back(get_id_of_file_as_number(li_li_path[iL][iP]));
 		li_li_id.push_back(li_id);
 	}
 	return li_li_id;
}



//#include <numeric>
template <typename T, typename Compare>
std::vector<std::size_t> sort_permutation(
    const std::vector<T>& vec,
    Compare compare)
{
    std::vector<std::size_t> p(vec.size());
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(),
        [&](std::size_t i, std::size_t j){ return compare(vec[i], vec[j]); });
    return p;
}


template <typename T>
void apply_permutation_in_place(
    std::vector<T>& vec,
    const std::vector<std::size_t>& p)
{
    std::vector<bool> done(vec.size());
    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        if (done[i])
        {
            continue;
        }
        done[i] = true;
        std::size_t prev_j = i;
        std::size_t j = p[i];
        while (i != j)
        {
            std::swap(vec[prev_j], vec[j]);
            done[j] = true;
            prev_j = j;
            j = p[j];
        }
    }
}

//-----------------------------------------------------------------------------------------------------  
//	vector<int> li_int({4, 1, 3, 2});
//	vector<string> li_string({"aa", "bb", "cc", "dd"});
//	sort_two_seqs_as_first_seq_sorted(li_int, li_string, true);
//	=> li_int : 1, 2, 3, 4 and li_string : "bb", "dd", "cc", "aa" 
//	sort_two_seqs_as_first_seq_sorted(li_int, li_string, false);
//	=> li_int : 4, 3, 2, 1 and li_string : "aa", "cc", "dd", "bb"  
	
void sort_two_seqs_as_first_seq_sorted(vector<int>& li_int, vector<string>& li_string, bool is_descending)
{
	//auto p = sort_permutation(li_int, [](int const& a, int const& b) {return is_descending ? a < b : a > b;});
	auto p = sort_permutation(li_int, [&is_descending](int const& a, int const& b) {return is_descending ? a < b : a > b;});
	//auto p = sort_permutation(li_int, [](int const& a, int const& b) {return a < b;});
	apply_permutation_in_place(li_int, p);
	apply_permutation_in_place(li_string, p);
}



//------------ Concatenate two images either horizontally or vetically	--------------  	
Mat concatenate_images(const Mat& img1, const Mat& img2, int minus_hori_plus_vert_zero_auto, int neg_left_or_top_0_center_pos_right_or_bottom, float factor_margin, const Scalar& color_margin)	
{	
	Mat res;	
	//  Check if the two image have the same # of channels and type	
	//  If # channels or type is different	
	//if(img1.type() != img2.type() || img1.channels() != img2.channels())    return res;	
	assert(img1.type() == img2.type() && img1.channels() == img2.channels());	
	int sum_row = img1.rows + img2.rows, sum_col = img1.cols + img2.cols;	
	int rows = img1.rows + img2.rows, cols = img1.cols + img2.cols, margin;	
	bool is_horizontal = true;
	Point offset_1(0, 0), offset_2(0, 0);
	if(minus_hori_plus_vert_zero_auto >= 0)	
	{	
		if(minus_hori_plus_vert_zero_auto > 0)	
		{	
			is_horizontal = false;	
		}	
		else if(cols > rows)	
		{	
			is_horizontal = false;	
		}	
	}	
	// Get dimension of final image	
	if(is_horizontal)	
	{	
		margin = sum_col * factor_margin;
		cols = sum_col + margin;
		rows = max(img1.rows, img2.rows);
		offset_2.x = img1.cols + margin;
		if(img1.rows > img2.rows)
		{
			int dif_row = img1.rows - img2.rows;
			rows = img1.rows;
			if(neg_left_or_top_0_center_pos_right_or_bottom > 0)
			{
				offset_2.y = dif_row;
			}
			else if(0 == neg_left_or_top_0_center_pos_right_or_bottom)
			{
				offset_2.y = dif_row * 0.5;
			}
		}
		else if(img1.rows < img2.rows)
		{
			int dif_row = img2.rows - img1.rows;
			rows = img2.rows;
			if(neg_left_or_top_0_center_pos_right_or_bottom > 0)
			{
				offset_1.y = dif_row;
			}
			else if(0 == neg_left_or_top_0_center_pos_right_or_bottom)
			{
				offset_1.y = dif_row * 0.5;
			}
		}			
	}	
	else	
	{	
		margin = sum_row * factor_margin; 
		cols = max(img1.cols, img2.cols);	
		rows = sum_row + margin;		
		offset_2.y = img1.rows + margin;		
		if(img1.cols > img2.cols)			
		{
			int dif_col = img1.cols - img2.cols;
			cols = img1.cols;
			if(neg_left_or_top_0_center_pos_right_or_bottom > 0)
			{
				offset_2.x = dif_col;
			}
			else if(0 == neg_left_or_top_0_center_pos_right_or_bottom)
			{
				offset_2.x = dif_col * 0.5;
			}
		}
		else if(img1.cols < img2.cols)
		{
			int dif_col = img2.cols - img1.rows;
			cols = img2.cols;
			if(neg_left_or_top_0_center_pos_right_or_bottom > 0)
			{
				offset_1.x = dif_col;
			}
			else if(0 == neg_left_or_top_0_center_pos_right_or_bottom)
			{
				offset_1.x = dif_col * 0.5;
			}
		}					
 	}	
 	// Create a black image	
 	//res = Mat3b(rows, cols, Vec3b(0,0,0));	
 	res = Mat::zeros(rows, cols, img1.type());	res = color_margin;
 	// Copy images in correct position	
 	img1.copyTo(res(Rect(offset_1.x, offset_1.y, img1.cols, img1.rows)));	
	img2.copyTo(res(Rect(offset_2.x, offset_2.y, img2.cols, img2.rows)));	
 	//imshow("img1", img1);waitKey();   imshow("img2", img2);   imshow("res", res); waitKey();  exit(0);	
 	return res;	
}	



//-----------------------------------------------------------------------------------------------------  
//	vector<vector<string>> li_li_path;
//	vector<string> li_path_1({ "/home/dir1/001.bmp", "/home/dir1/002.bmp", "/home/dir1/003.bmp" });	
//	vector<string> li_path_2({ "/home/dir2/000.bmp", "/home/dir2/002.bmp", "/home/dir2/003.bmp" });	
//	vector<string> li_path_3({ "/home/dir3/002.bmp", "/home/dir3/003.bmp", "/home/dir3/004.bmp" });
//	li_li_path.push_back(li_path_1);	li_li_path.push_back(li_path_2);	li_li_path.push_back(li_path_3);
//	if(are_all_seqs_gray(li_li_path))
//	{
//		....		

bool are_all_seqs_gray(const vector<vector<string> >& li_li_path)
{
	bool is_all_gray = true;
 	for(int iL = 0; iL < li_li_path.size(); iL++)
 	{
 		Mat im = imread(li_li_path[iL][0], CV_LOAD_IMAGE_UNCHANGED);
 		if(im.channels() > 1) { is_all_gray = false; break; }
 	}
	return is_all_gray;
}





			
//-----------------------------------------------------------------------------------------------------  
//	vector<vector<string>> li_li_path;
//	vector<string> li_path_1({ "/home/dir1/001.bmp", "/home/dir1/002.bmp", "/home/dir1/003.bmp" });	
//	vector<string> li_path_2({ "/home/dir2/000.bmp", "/home/dir2/002.bmp", "/home/dir2/003.bmp" });	
//	vector<string> li_path_3({ "/home/dir3/002.bmp", "/home/dir3/003.bmp", "/home/dir3/004.bmp" });
//	li_li_path.push_back(li_path_1);	li_li_path.push_back(li_path_2);	li_li_path.push_back(li_path_3);
//	concatenate_images_from_seqeunces_into_video_or_sequence(li_li_path, -1, true, "/home/dir4/");
//	=> The video of concatenated images are saved at : /home/dir4/output.avi
//	=> The first frame of the video is composed of "/home/dir1/002.bmp", "/home/dir2/002.bmp" and "/home/dir2/002.bmp".   
//	=> The second(which is the last) frame of the video is composed of "/home/dir1/003.bmp", "/home/dir2/003.bmp" and "/home/dir3/003.bmp".   

void concatenate_images_from_seqeunces_into_video_or_sequence(vector< vector<string> >& li_li_path, 
							      int hori_minus_vert_plus, int neg_left_or_top_0_center_pos_right_or_bottom,
							      float factor_margin, const Scalar& color_margin, 
                                  int neg_vid_0_seq_pos_gif, const string& dir_save)
{
	vector<vector<int> > li_li_id = get_list_of_list_of_ids_as_number(li_li_path);
	for(int iS = 0; iS < li_li_id.size(); iS++) sort_two_seqs_as_first_seq_sorted(li_li_id[iS], li_li_path[iS], true);
	bool is_gray = are_all_seqs_gray(li_li_path);
	int /*w_concat, h_concat,*/ iF = 0, n_seq = li_li_path.size(), n_frm = -1, cv_read_flag = is_gray ? CV_LOAD_IMAGE_GRAYSCALE : CV_LOAD_IMAGE_COLOR;
	if (n_seq > 0) n_frm = li_li_path[0].size();
    vector<int> li_idx(n_seq, -1);
	VideoWriter vw;
	string path_vid_or_gif; //= python_join_equivalent(dir_save, "output.avi");
    //GifWriter writer = {};
    if(0 > neg_vid_0_seq_pos_gif)
    {
        path_vid_or_gif = python_join_equivalent(dir_save, "output.avi");
    }
    else if(0 < neg_vid_0_seq_pos_gif)
    {
        //path_vid_or_gif = python_join_equivalent(dir_save, "output.gif");
        path_vid_or_gif = python_join_equivalent(dir_save, "output.webp");
    }

	//mkdir_if_not_exist(dir_save.c_str());
	mkdirs(dir_save.c_str());
    while(get_next_index(li_idx, li_li_id))
	{
        cout << "iF : " << iF << " / " << n_frm << endl;
        int idx = li_idx[0];
		string fn = li_li_path[0][idx];
		Mat im_cur = imread(fn, cv_read_flag);
		for(int iS = 1; iS < n_seq; iS++)
		{
			idx = li_idx[iS];
			fn = li_li_path[iS][idx];
			//im_cur = concatenate_images(im_cur, imread(fn, cv_read_flag), hori_minus_vert_plus);
			im_cur = concatenate_images(im_cur, imread(fn, cv_read_flag), hori_minus_vert_plus, neg_left_or_top_0_center_pos_right_or_bottom, 
						    factor_margin, color_margin);	

		}
        if(0 > neg_vid_0_seq_pos_gif) vw = write_one_frame_to_video(vw, im_cur, 0 == iF, path_vid_or_gif, 30, 1000);
		else if(0 <= neg_vid_0_seq_pos_gif) 
        {
            //string fn_concat = "concat_" + get_exact_file_name_from_path(fn) + ".bmp"; 
            string fn_concat = "concat_" + get_exact_file_name_from_path(fn) + ".png"; 
            save_one_image_under_directory(im_cur, dir_save, fn_concat);
        }     
        /*
        else
        {
            Mat im_rgba;
            cv::cvtColor(im_cur, im_rgba, is_gray ? CV_GRAY2RGBA : CV_BGR2RGBA);
            if (0 == iF)
            {
                w_concat = im_rgba.size().width;  h_concat = im_rgba.size().height;
                GifBegin( &writer, path_vid_or_gif.c_str(), w_concat, h_concat, 10, 8, true );
            }
            //GifWriteFrame(&writer, im_rgba.data, im_rgba.size().width, im_rgba.size().height, 20);
            GifWriteFrame(&writer, im_rgba.data, w_concat, h_concat, 10, 8, true);
        } 
        */
		iF++;
	}
	if(0 > neg_vid_0_seq_pos_gif) 
    { 
        cout << "concatenated video has just saved at : " << path_vid_or_gif << endl;  
        vw.release(); 
    }
    else if(0 < neg_vid_0_seq_pos_gif)
    {
#if 0    
        string str_imagemagick = "convert -delay 20 -loop 0 -monitor " + python_join_equivalent(dir_save, "concat_*") + " " + path_vid_or_gif;
        cout << "str_imagemagick : " << str_imagemagick << endl;    //exit(0);
        std::system(str_imagemagick.c_str());  
        cout << "concatenated animated-gif has just saved at : " << path_vid_or_gif << endl;    //exit(0);  
        string str_optimize_10 = "convert " + path_vid_or_gif + " -fuzz 10% -layers Optimize " + path_vid_or_gif;
        cout << "str_optimize_10 : " << str_optimize_10 << endl;    //exit(0);
        std::system(str_optimize_10.c_str());  
#else   //0

        string str_webp = "img2webp -v -mixed -d 40 " + python_join_equivalent(dir_save, "concat_*.png") + " -o " + path_vid_or_gif;
        cout << "str_webp : " << str_webp << endl;    //exit(0);
        std::system(str_webp.c_str());    //exit(0);
 
        
        /*        
        string str_ffmpeg = "ffmpeg -y -framerate 15 -pattern_type glob -i '" + python_join_equivalent(dir_save, "concat_*.bmp") + "' " + path_vid_or_gif;
        cout << "str_ffmpeg : " << str_ffmpeg << endl;    //exit(0);
        std::system(str_ffmpeg.c_str());    //exit(0);
       
        string str_magick_15 = "convert " + path_vid_or_gif + " -fuzz 15% -layers Optimize magick_15.gif ";
        cout << "str_magick_15 : " << str_magick_15 << endl;    //exit(0);
        std::system(str_magick_15.c_str());  


        //string str_gifsicle = "gifsicle -O3 " + path_vid_or_gif + " -o " + path_vid_or_gif;
        string str_gifsicle = "gifsicle -O3 " + path_vid_or_gif + " -o gifsicle_O3.gif";
        cout << "str_gifsicle : " << str_gifsicle << endl;    //exit(0);
        std::system(str_gifsicle.c_str());
        */

        /*
        str_gifsicle = "gifsicle -O2 " + path_vid_or_gif + " -o gifsicle_O2.gif";
        cout << "str_gifsicle : " << str_gifsicle << endl;    //exit(0);
        std::system(str_gifsicle.c_str());

        str_gifsicle = "gifsicle -O1 " + path_vid_or_gif + " -o gifsicle_O1.gif";
        cout << "str_gifsicle : " << str_gifsicle << endl;    //exit(0);
        std::system(str_gifsicle.c_str());
        */
#endif  //0
        //GifEnd( &writer );
    }
    else
    {
        cout << "concatenated image sequence has just saved under : " << dir_save << endl;
    }
    return;
}


//------------ get_list_of_image_path_under_this_directory --------------
//	vector<string> li_fn_img = li_get_list_of_image_path_under_this_directory("/home/someuser/somefolder/", 412, 414);
//	for(int i = 0; i < li_fn_img.size(); i++) cout << li_fn_img[i] << " ";
//	=> /home/someuser/somefolder/img412.bmp /home/someuser/somefolder/img413.png /home/someuser/somefolder/img414.jpg 
#include <experimental/filesystem>  // this requires to use "-lstdc++fs" as a library in Makefile.
namespace fs = std::experimental::filesystem;
vector<string> get_list_of_image_path_under_this_directory(const string& dir_img, int id_frm_start, int id_frm_last)
{
    //cout << "dir_img : " << dir_img << endl;    //exit(0);
	vector<string> li;
	for(auto& p: fs::recursive_directory_iterator(dir_img))
	{
		string str_path = p.path().string();
        //cout << "id_frm_start : " << id_frm_start << endl; 
        //cout << "id_frm_last : " << id_frm_last << endl;    //exit(0);
		if(!is_image_file(str_path)) continue;
		int aidi = get_id_of_file_as_number(str_path);// < id_frm_start) continue;
        //cout << "aidi : " << aidi << endl;    //exit(0);
		if(id_frm_start >= 0 &&  aidi < id_frm_start) 
        {
            continue;
        }     
		if(id_frm_last >= 0 && id_frm_last >= id_frm_start &&  aidi > id_frm_last)
        {
            continue;
        }     
		li.push_back(str_path);
	}
	sort(li.begin(), li.end());
    //cout << "li.size() : " << li.size() << endl;    exit(0);
	return li;
}
/*
void create_animated_gif_from_seq(const string& dir, const string& path_gif) 
{
    GifWriter writer = {};
    GifBegin(&writer, path_gif.c_str(), frame.size().width, frame.size().height, 20);
    uint frameCounter = 0;
    while(success && frameCounter<=15){
        success = video.read(frame);
        GifWriteFrame(&writer,frame.data, frame.size().width, frame.size().height, 20);
        frameCounter++;
    }
    GifEnd(&writer);
}
*/

int main(int argc, char *argv[]) 
{
    vector<vector<string>> li_li_path;
    int neg_vid_0_seq_pos_gif = atoi(argv[1]);
    string dir_res = argv[2];
    int hori_minus_vert_plus = atoi(argv[3]);
    int neg_left_or_top_0_center_pos_right_or_bottom = atoi(argv[4]);
    float factor_margin = atof(argv[5]);//0.01;
    int margin_color = atoi(argv[6]);
    int frm_start  = atoi(argv[7]);
    int frm_end = atoi(argv[8]);
    cv::Scalar color_margin(margin_color, margin_color, margin_color);
    cout << "AAA" << endl;
    for(int iA = 9; iA < argc; iA++)
    {
        string dir = argv[iA];
        cout << "dir : " << dir << endl;
        vector<string> li_path = get_list_of_image_path_under_this_directory(dir, frm_start, frm_end); 
        li_li_path.push_back(li_path);
    }   
    //cout << "li_li_path.size() : " << li_li_path.size() << endl;
    //cout << "li_li_path[0].size() : " << li_li_path[0].size() << endl;
    //cout << "BBB" << endl;
//	vector<string> li_path_1({ "/home/dir1/001.bmp", "/home/dir1/002.bmp", "/home/dir1/003.bmp" });	
//	vector<string> li_path_2({ "/home/dir2/000.bmp", "/home/dir2/002.bmp", "/home/dir2/003.bmp" });	
//	vector<string> li_path_3({ "/home/dir3/002.bmp", "/home/dir3/003.bmp", "/home/dir3/004.bmp" });
//	li_li_path.push_back(li_path_1);	li_li_path.push_back(li_path_2);	li_li_path.push_back(li_path_3);
    //bool save_as_video = neg_vid_pos_gif < 0;
    concatenate_images_from_seqeunces_into_video_or_sequence(li_li_path, hori_minus_vert_plus,
        neg_left_or_top_0_center_pos_right_or_bottom, factor_margin, color_margin, neg_vid_0_seq_pos_gif, dir_res);
        
//	=> The video of concatenated images are saved at : /home/dir4/output.avi
//	=> The first frame of the video is composed of "/home/dir1/002.bmp", "/home/dir2/002.bmp" and "/home/dir2/002.bmp".   
//	=> The second(which is the last) frame of the video is composed of "/home/dir1/003.bmp", "/home/dir2/003.bmp" and "/home/dir3/003.bmp".   

}
