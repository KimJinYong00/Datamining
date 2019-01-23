#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include<map>
#include<set>

using namespace std;

string make_feature(string);
void make_feature1(string);
void sort_remove_overlap(string, string);
void cal_chi(string, string);
void cal_chi();
void reconstruct_chi(string, string);
void sort_chi(string, string);
void feature_to_chi(string, string);
void final_sort(string, string);
void make_chi_study(string, string);
double max(double *);
bool compare(string, string);
bool compare1(string, string);
void abc();

int b = 0, cat[8] = { 0 };//기사수 카테고리 수 확인
set<string> s;
set<string> chi_set;
pair<set<string>::iterator, bool> pr;
set<string>::iterator iter;
//string arr[2500000];
//int doc[2][2500000];

/*
txt 1 ~ 4
total : 15978
CAT 0 : 348
CAT 1 : 5023
CAT 2 : 539
CAT 3 : 479
CAT 4 : 2362
CAT 5 : 3637
CAT 6 : 3309
CAT 7 : 281
*/

void main() {
	string output;
	ofstream outFile;

	/*output += make_feature("HKIB-20000_001.txt");
	output += make_feature("HKIB-20000_002.txt");
	output += make_feature("HKIB-20000_003.txt");
	output += make_feature("HKIB-20000_004.txt");

	outFile.open("feature.txt");
	outFile << output;
	outFile.close();

	sort_remove_overlap("feature.txt","feature_sort_remove_overlap.txt");
	cal_chi("feature_sort_remove_overlap.txt", "chi.txt");
	sort_chi("chi.txt", "chi_sorted.txt");*/
	//reconstruct_chi("feature_sort_remove_overlap.txt", "feature_reconstruct.txt");
	//feature_to_chi("feature_reconstruct.txt", "replace_chi.txt");
	/*final_sort("replace_chi.txt", "chi_final.txt");
	make_chi_study("chi_final", "chi_studying.txt");

	output = "";
	output += make_feature("HKIB-20000_005.txt");

	outFile.open("test_feature.txt");
	outFile << output;
	outFile.close();

	sort_remove_overlap("test_feature.txt", "test_feature_sort_remove.txt");
	reconstruct_chi("test_feature_sort_remove.txt", "test_feature_reconstruct.txt");
	feature_to_chi("test_feature_reconstruct.txt", "test_replace_chi.txt");
	final_sort("test_replace_chi.txt", "test_chi_final.txt");
	make_chi_study("test_chi_final.txt", "test_chi_studying.txt");*/
	//abc();

	make_feature1("HKIB-20000_001.txt");
	make_feature1("HKIB-20000_002.txt");
	make_feature1("HKIB-20000_003.txt");
	make_feature1("HKIB-20000_004.txt");
	cout << "카이스퀘어" << endl;
	cal_chi();
	/*cout << b << endl;
	cout << "파일쓰기" << endl;
	outFile.open("aaa.txt");
	for (iter = s.begin(); iter != s.end(); ++iter)
		outFile << *iter << endl;
	outFile.close();*/
}

void make_feature1(string file_name) {
	ifstream inFile;
	char a[4000];
	int category = 0;
	string output;
	char* feature = NULL;
	char *context = NULL;
	char token[] = " ~-_'/.,<>:[]{}()!?*`&^;=$+|\"#";

	inFile.open(file_name);

	if (inFile.is_open()) {
		cout << "file open for reading" << endl;
		while (inFile.getline(a, sizeof(a))) {
			if (strstr(a, "@DOCUMENT")) {
				b++;
				continue;
			}

			if (strstr(a, "<KW>")) {
				while (inFile.getline(a, sizeof(a))) {
					if (strstr(a, "@DOCUMENT"))
						b++;
					break;
				}
			}

			if (strstr(a, "#DocID") || strstr(a, "#TITLE") || strstr(a, "#CAT'07"))
				continue;

			if (strstr(a, "#CAT'03:")) {
				feature = strtok_s(a, "/", &context);
				while (feature) {
					if (strcmp(feature, "건강과 의학") == 0) {
						category = 1;
						cat[0]++;
						break;
					}
					else if (strcmp(feature, "경제") == 0) {
						category = 2;
						cat[1]++;
						break;
					}
					else if (strcmp(feature, "과학") == 0) {
						category = 3;
						cat[2]++;
						break;
					}
					else if (strcmp(feature, "교육") == 0) {
						category = 4;
						cat[3]++;
						break;
					}
					else if (strcmp(feature, "문화와 종교") == 0) {
						category = 5;
						cat[4]++;
						break;
					}
					else if (strcmp(feature, "사회") == 0) {
						category = 6;
						cat[5]++;
						break;
					}
					else if (strcmp(feature, "산업") == 0) {
						category = 7;
						cat[6]++;
						break;
					}
					else if (strcmp(feature, "여가생활") == 0) {
						category = 8;
						cat[7]++;
						break;
					}
					feature = strtok_s(NULL, "/", &context);
				}
				continue;
			}

			feature = strtok_s(a, token, &context);
			while (feature) {
				if (strstr(feature, "TEXT")) {
					feature = strtok_s(NULL, token, &context);
					continue;
				}
				output += feature;
				output += " ";
				output += to_string(category);
				output += " ";
				output += to_string(b);
				pr = s.insert(output);
				output = "";
				feature = strtok_s(NULL, token, &context);
			}
		}
	}
	inFile.close();
}

string make_feature(string file_name) {
	ifstream inFile;
	char a[4000];
	int category = 0;
	string output;
	char* feature = NULL;
	char *context = NULL;
	char token[] = " ~-_'/.,<>:[]{}()!?*`&^;=$+|\"#";

	inFile.open(file_name);

	if (inFile.is_open()) {
		cout << "file open for reading" << endl;
		while (inFile.getline(a, sizeof(a))) {
			if (strstr(a, "@DOCUMENT")) {
				b++;
				continue;
			}

			if (strstr(a, "<KW>")) {
				while (inFile.getline(a, sizeof(a))) {
					if (strstr(a, "@DOCUMENT"))
						break;
				}
			}

			if (strstr(a, "#DocID") || strstr(a, "#TITLE") || strstr(a, "#CAT'07"))
				continue;

			if (strstr(a, "#CAT'03:")) {
				feature = strtok_s(a, "/", &context);
				while (feature) {
					if (strcmp(feature, "건강과 의학") == 0) {
						category = 1;
						cat[0]++;
						break;
					}
					else if (strcmp(feature, "경제") == 0) {
						category = 2;
						cat[1]++;
						break;
					}
					else if (strcmp(feature, "과학") == 0) {
						category = 3;
						cat[2]++;
						break;
					}
					else if (strcmp(feature, "교육") == 0) {
						category = 4;
						cat[3]++;
						break;
					}
					else if (strcmp(feature, "문화와 종교") == 0) {
						category = 5;
						cat[4]++;
						break;
					}
					else if (strcmp(feature, "사회") == 0) {
						category = 6;
						cat[5]++;
						break;
					}
					else if (strcmp(feature, "산업") == 0) {
						category = 7;
						cat[6]++;
						break;
					}
					else if (strcmp(feature, "여가생활") == 0) {
						category = 8;
						cat[7]++;
						break;
					}
					feature = strtok_s(NULL, "/", &context);
				}
				continue;
			}

			feature = strtok_s(a, token, &context);
			while (feature) {
				if (strstr(feature, "TEXT")) {
					feature = strtok_s(NULL, token, &context);
					continue;
				}
				output += feature;
				output += " ";
				output += category;
				output += " ";
				output += to_string(b);
				output += "\n";
				pr = s.insert(output);
				if (pr.second == true)

					feature = strtok_s(NULL, token, &context);
			}
		}
	}
	inFile.close();

	return output;
}

void sort_remove_overlap(string file_name, string result_name) {
	ifstream inFile;
	ofstream outFile;

	string out[3000000];
	char temp[300];
	int cnt = 0, i = 0;
	string t;

	inFile.open(file_name);

	if (inFile.is_open()) {
		cout << "Feature file open" << endl;
		while (inFile.getline(temp, sizeof(temp))) {
			out[cnt++] = temp;
		}
	}
	cout << "read finish" << endl;
	inFile.close();


	cout << "sort start" << endl;
	sort(out, out + cnt);
	cout << "sort finish" << endl;


	i = 0;
	t = out[i];

	outFile.open(result_name);
	while (i < cnt) {
		if (t.compare(out[i]) != 0) {
			outFile << t << endl;
			t = out[i];
		}
		i++;
	}
	outFile.close();
}

void cal_chi() {
	double total = 15978.0;//전체 카테고리 합
	//double cat_total[8] = { 348.0, 5023.0, 539.0, 479.0, 2362.0, 3637.0, 3309.0, 281.0 };//카테고리별 총 개수
	double cat_num[8] = { 0.0 };//단어 출현한 카테고리 개수
	double chi[8] = { 0.0 };//카이스퀘어 값 8개
	double A, B, C, D;
	char *context = NULL;
	char *word = NULL;
	string out, before;
	int a = 0, b = 0;
	ofstream outFile;

	for (iter = s.begin(); iter != s.end(); iter++) {
		out = *iter;
		word = strtok_s((char *)out.c_str(), " ", &context);
		b = atoi(strtok_s(NULL, " ", &context));
		if (a == 0) {
			before = word;//단어 나온 카테고리 +1
			a = 1;
		}

		if (before.compare(word) != 0) {//카이스퀘어 계산하기
			//cout << before << " " << cat_num[0] << " " << cat_num[1] << " " << cat_num[2] << " " << cat_num[3] << " " << cat_num[4] << " " << cat_num[5] << " " << cat_num[6] << " " << cat_num[7] << " " << endl;
			//cout << max(chi) << endl;
			A = 0.0; B = 0.0; C = 0.0; D = 0.0;
			for (int i = 0; i < 8; i++) {
				A = cat_num[i];
				B = cat[i] - A;
				for (int j = 0; j < 8; j++) {
					if (j == i)
						continue;
					C += cat_num[j];
				}
				D = (total - cat[i]) - C;
				chi[i] = total * (((A * D) - (C * B)) * ((A * D) - (C * B)))  * (1.0 / (A + B)) * (1.0 / (B + D)) * (1.0 / (A + C)) * (1.0 / (C + D));
				C = 0.0;
			}
			//cout << chi[0] << " " << chi[1] << " " << chi[2] << " " << chi[3] << " " << chi[4] << " " << chi[5] << " " << chi[6] << " " << chi[7] << " " << endl;
			cout << max(chi) << endl;
			chi_set.insert(before + " " + to_string(max(chi)));

			for (int i = 0; i < 8; i++) {
				chi[i] = 0.0;
				cat_num[i] = 0.0;
			}
			before = word;
		}
		cat_num[b]++;//단어 나온 카테고리 +1
	}
	/*cout << "파일쓰기" << endl;
	outFile.open("chi1.txt");
	for (iter = chi_set.begin(); iter != chi_set.end(); ++iter)
		outFile << *iter << endl;
	outFile.close(); 
	*/
}

void cal_chi(string file_name, string result_name) {
	ifstream inFile;
	ofstream outFile;
	int cnt = 0, first = 0, k = 0;
	double total = 15978.0;//전체 카테고리 합
	double cat_total[8] = { 348.0, 5023.0, 539.0, 479.0, 2362.0, 3637.0, 3309.0, 281.0 };//카테고리별 총 개수
	double cat_num[8] = { 0.0, };//단어 출현한 카테고리 개수
	double chi[8] = { 0.0, };//카이스퀘어 값 8개
	double A = 0.0, B = 0.0, C = 0.0, D = 0.0;
	char temp[600];
	string out, word, before, result[600000];
	char *feature = NULL, *cat = NULL;
	char *context = NULL;
	char token[] = " ";

	//inFile.open("feature_remove_overlap.txt");
	inFile.open(file_name);

	if (inFile.is_open()) {
		cout << "file open for chi-squre" << endl;
		while (inFile.getline(temp, sizeof(temp))) {
			feature = strtok_s(temp, token, &context);
			if (first == 0) {
				out = feature;
				first = 1;
			}
			if (out.compare(feature) != 0) {
				for (int i = 0; i < 8; i++) {
					A = cat_num[i];
					B = cat_total[i] - A;
					for (int j = 0; j < 8; j++) {
						if (j == i)
							continue;
						C += cat_num[j];
					}
					D = (total - cat_total[i]) - C;
					chi[i] = total * (((A * D) - (C * B)) * ((A * D) - (C * B)))  * (1.0 / (A + B)) * (1.0 / (B + D)) * (1.0 / (A + C)) * (1.0 / (C + D));
					C = 0.0;
				}

				result[k] += to_string(max(chi));
				result[k] += " ";
				result[k++] += out;

				for (int i = 0; i < 8; i++) {
					chi[i] = 0.0;
					cat_num[i] = 0.0;
				}
				A = 0.0; B = 0.0; C = 0.0; D = 0.0;
				out = feature;
			}
			while (feature) {
				feature = strtok_s(NULL, token, &context);
				cat_num[atoi(feature) - 1]++;
				break;
			}
		}
	}
	inFile.close();

	cout << "sort strat" << endl;
	sort(result, result + k, compare);
	cout << "sort finish" << endl;

	int i = k - 1;
	//outFile.open("chi.txt");
	outFile.open(result_name);
	int j = 1;
	while (i >= 0) {
		outFile << j++ << ":";
		outFile << result[i--] << endl;
	}
	outFile.close();
}

void reconstruct_chi(string file_name, string result_name) {
	ifstream inFile;
	ofstream outFile;
	string out[3000000], s[3];
	char temp[600];
	char *feature, *c;
	char *context = NULL;
	int cnt = 0, i = 0, j = 0;

	//inFile.open("feature_remove_overlap.txt");
	inFile.open(file_name);

	if (inFile.is_open()) {
		cout << "File open for reconstruction" << endl;
		while (inFile.getline(temp, sizeof(temp))) {
			out[cnt++] = temp;
		}
	}

	cout << "Reconstruct" << endl;

	while (i <= cnt) {
		c = (char *)out[i].c_str();
		feature = strtok_s(c, " ", &context);
		while (feature) {
			s[j++] = feature;
			feature = strtok_s(NULL, " ", &context);
		}
		out[i++] = s[2] + " " + s[1] + " " + s[0];
		s[2] = "";
		s[1] = "";
		s[0] = "";
		j = 0;
	}
	inFile.close();

	cout << "sort start" << endl;
	sort(out, out + cnt, compare);
	cout << "sort finish" << endl;
	//outFile.open("feature_overlap_sorted.txt");

	outFile.open(result_name);
	i = 0;
	while (i <= cnt)
		outFile << out[i++] << endl;

	outFile.close();
}

void sort_chi(string file_name, string result_name) {
	ifstream inFile;
	ofstream outFile;
	string out[2500000], s[3];
	char temp[600];
	char *feature, *c;
	char *context = NULL;
	int cnt = 0, i = 0, j = 0;

	//inFile.open("chi.txt");
	inFile.open(file_name);

	if (inFile.is_open()) {
		cout << "File open for chi-squre sorting" << endl;
		while (inFile.getline(temp, sizeof(temp))) {
			out[cnt++] = temp;
		}
	}

	while (i <= cnt) {
		c = (char *)out[i].c_str();
		feature = strtok_s(c, " ", &context);
		while (feature) {
			s[j++] = feature;
			feature = strtok_s(NULL, " ", &context);
		}
		out[i++] = s[2] + " " + s[0] + " " + s[1];
		s[2] = "";
		s[1] = "";
		s[0] = "";
		j = 0;
	}
	inFile.close();

	cout << "sort start" << endl;
	sort(out, out + cnt);
	cout << "sort finish" << endl;
	//outFile.open("chi_sorted.txt");
	outFile.open(result_name);
	i = 0;
	while (i <= cnt)
		outFile << out[i++] << endl;

	outFile.close();
}

void feature_to_chi(string file_name1, string result_name) {
	map<string, string> map;
	ifstream inFile;
	ofstream outFile;
	string out[2500000], b, s;
	string chi[570000][3];
	char temp[100000];
	char *feature = NULL, *context = NULL;
	int cnt_feat = 0, cnt_chi = 0, i = 0, j = 0;
	bool check = 0;

	//inFile.open("feature_overlap_sorted.txt");
	inFile.open(file_name1);

	if (inFile.is_open()) {
		cout << "File open for feature to chi-squre" << endl;
		while (inFile.getline(temp, sizeof(temp))) {
			feature = strtok_s(temp, " ", &context);
			feature = strtok_s(NULL, " ", &context);
			feature = strtok_s(NULL, " ", &context);
			out[cnt_feat++] = feature;
		}
	}

	inFile.close();

	inFile.open("chi_sorted.txt");

	string c1, c2;
	if (inFile.is_open()) {
		cout << "File open for feature to chi-squre" << endl;
		while (inFile.getline(temp, sizeof(temp))) {
			//c1 = strtok_s("temp", " ", &context);
			//c2 = strtok_s(NULL, " ", &context);
			chi[cnt_chi][0] = strtok_s(temp, " ", &context);
			chi[cnt_chi][1] = strtok_s(NULL, " ", &context);
			chi[cnt_chi++][2] = strtok_s(NULL, " ", &context);
			//map.insert(pair<string, string>(c1, c2));
		}
	}

	inFile.close();

	cout << "search" << endl;
	int first = 0, last = cnt_chi - 1, half;
	while (i < cnt_feat) {
		while (first <= last) {
			half = (first + last) / 2;
			b = chi[half][0];
			if (out[i].compare(b) == 0) {
				check = 1;
				break;
			}
			else if (out[i].compare(b) < 0)
				last = half - 1;
			else if (out[i].compare(b) > 0)
				first = half + 1;
		}
		if (check == 0) {
			first = 0;
			last = cnt_chi - 1;
			out[i++] = "";
			continue;
		}
		first = 0;
		last = cnt_chi - 1;
		check = 0;
		out[i++] = chi[half][1] + ":" + chi[half][2];
	}
	cout << "search finish" << endl;

	i = 0;
	inFile.open(file_name1);

	if (inFile.is_open()) {
		cout << "File open for feature to chi-squre" << endl;
		while (inFile.getline(temp, sizeof(temp))) {
			if (out[i].compare("") == 0) {
				i++;
				continue;
			}
			feature = strtok_s(temp, " ", &context);
			s = feature;
			s += " ";
			feature = strtok_s(NULL, " ", &context);
			s += feature;
			s += " ";
			s += out[i];
			out[i++] = s;
		}
	}

	inFile.close();
	i = 0;

	//outFile.open("feature_chi_sorted.txt");
	outFile.open(result_name);
	while (i < cnt_feat) {
		if (out[i].compare("") == 0) {
			i++;
			continue;
		}
		outFile << out[i++] << endl;
	}
	outFile.close();
}

void final_sort(string file_name, string result_name) {
	ifstream inFile;
	ofstream outFile;
	string out[2500000];
	int cnt = 0;
	char temp[1024];
	int* index;
	//inFile.open("feature_chi_sorted.txt");
	inFile.open(file_name);
	if (inFile.is_open()) {
		cout << "File open for final sorting" << endl;
		while (inFile.getline(temp, sizeof(temp))) {
			out[cnt++] = temp;
		}
	}

	inFile.close();
	cout << "start sorting" << endl;
	sort(out, out + cnt, compare1);
	cout << "finish sorting" << endl;


	//outFile.open("feature_chi_final_sorted.txt");
	outFile.open(result_name);

	int i = 0;
	string t = out[0];
	while (i < cnt) {
		if (t.compare(out[i]) != 0) {
			t = out[i];
			outFile << out[i] << endl;
		}
		i++;
	}
	outFile.close();
}

void make_chi_study(string file_name, string result_name) {
	ifstream inFile;
	ofstream outFile;
	string out[16000], s[3], t, before;
	char *feature = NULL, *context = NULL;
	char temp[600];
	int cnt = 0, first = 0, i;

	//inFile.open("feature_chi_final_sorted.txt");
	inFile.open(file_name);
	if (inFile.is_open()) {
		cout << "File open for make studying file" << endl;
		while (inFile.getline(temp, sizeof(temp))) {
			if (first == 0) {
				s[0] = strtok_s(temp, " ", &context);
				s[1] = strtok_s(NULL, " ", &context);
				if (s[1].compare("0") == 0)
					s[1] = "1";
				else if (s[1].compare("1") == 0)
					s[1] = "2";
				else if (s[1].compare("2") == 0)
					s[1] = "3";
				else if (s[1].compare("3") == 0)
					s[1] = "4";
				else if (s[1].compare("4") == 0)
					s[1] = "5";
				else if (s[1].compare("5") == 0)
					s[1] = "6";
				else if (s[1].compare("6") == 0)
					s[1] = "7";
				else if (s[1].compare("7") == 0)
					s[1] = "8";
				s[2] = strtok_s(NULL, " ", &context);
				before = s[2];
				out[cnt] = s[1] + " " + s[2];
				first++;
				continue;
			}

			t = strtok_s(temp, " ", &context);
			if (s[0].compare(t) != 0) {
				s[0] = t;
				s[1] = strtok_s(NULL, " ", &context);
				if (s[1].compare("0") == 0)
					s[1] = "1";
				else if (s[1].compare("1") == 0)
					s[1] = "2";
				else if (s[1].compare("2") == 0)
					s[1] = "3";
				else if (s[1].compare("3") == 0)
					s[1] = "4";
				else if (s[1].compare("4") == 0)
					s[1] = "5";
				else if (s[1].compare("5") == 0)
					s[1] = "6";
				else if (s[1].compare("6") == 0)
					s[1] = "7";
				else if (s[1].compare("7") == 0)
					s[1] = "8";
				s[2] = strtok_s(NULL, " ", &context);
				if (before.compare(s[2]) != 0) {
					before = s[2];
					out[++cnt] = s[1] + " " + s[2];
				}
			}
			else {
				s[2] = strtok_s(NULL, " ", &context);
				s[2] = strtok_s(NULL, " ", &context);
				if (before.compare(s[2]) != 0) {
					before = s[2];
					out[cnt] += " ";
					out[cnt] += s[2];
				}
			}
		}
	}
	cout << "finish reading" << endl;
	cout << cnt << endl;
	inFile.close();


	//outFile.open("chi_studying.txt");
	outFile.open(result_name);
	i = 0;
	while (i <= cnt) {
		outFile << out[i++] << endl;
	}
	outFile.close();

}

bool compare(string s1, string s2) {
	char *context = NULL;
	char *a = (char *)s1.c_str();
	char *b = (char *)s2.c_str();
	a = strtok_s(a, " ", &context);
	b = strtok_s(b, " ", &context);

	if (atof(a) < atof(b))
		return true;
	else
		return false;
}

bool compare1(string s1, string s2) {
	char *context = NULL;
	char *t1 = (char *)s1.c_str();
	char *t2 = (char *)s2.c_str();
	char *a1, *a2, *b1, *b2;
	a1 = strtok_s(t1, " ", &context);
	a2 = strtok_s(NULL, " ", &context);
	a2 = strtok_s(NULL, " :", &context);
	b1 = strtok_s(t2, " ", &context);
	b2 = strtok_s(NULL, " ", &context);
	b2 = strtok_s(NULL, " :", &context);

	if (atol(a1) == atol(b1)) {
		if (atol(a2) < atol(b2))
			return true;
		else
			return false;
	}
	else if (atol(a1) < atol(b1))
		return true;
	else
		return false;
}

double max(double* chi) {
	double max = chi[0];
	for (int i = 0; i < 8; i++) {
		if (max < chi[i])
			max = chi[i];
	}
	return max;
}