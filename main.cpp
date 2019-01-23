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
void reconstruct_chi(string, string);
void sort_chi(string, string);
void feature_to_chi(string, string);
void final_sort(string, string);
void make_chi_study(string, string);
double max(double *);
bool compare(string, string);
bool compare1(string, string);

//"�ǰ��� ����", "����", "����", "����", "��ȭ�� ����", "��ȸ", "���", "������Ȱ"

int b = 0, cat1 = 0, cat2 = 0, cat3 = 0, cat4 = 0, cat5 = 0, cat6 = 0, cat7 = 0, cat0 = 0;//���� ī�װ� �� Ȯ��

void main() {
	string output;
	ofstream outFile;

	output += make_feature("HKIB-20000_001.txt");//1������ �б�
	output += make_feature("HKIB-20000_002.txt");//2������ �б�
	output += make_feature("HKIB-20000_003.txt");//3������ �б�
	output += make_feature("HKIB-20000_004.txt");//4������ �б�

	outFile.open("feature.txt");//��ó�� ���Ϸ� ����
	outFile << output;
	outFile.close();

	sort_remove_overlap("feature.txt","feature_sort_remove_overlap.txt");//��ó ����
	cal_chi("feature_sort_remove_overlap.txt", "chi.txt");//���õ� ��ó�� �̿��Ͽ� ī�̽����� ���ϱ�
	sort_chi("chi.txt", "chi_sorted.txt");//ī�̽����� �� ����
	reconstruct_chi("feature_sort_remove_overlap.txt", "feature_reconstruct.txt");//���õ� ī�̽����� ���� ���Ĺٲٱ�
	feature_to_chi("feature_reconstruct.txt", "replace_chi.txt");//����� �ܾ��� ī�̽����� ġȯ
	final_sort("replace_chi.txt", "chi_final.txt");//ġȯ�� �� �������� ����
	make_chi_study("chi_final", "chi_studying.txt");//SVM���� ���� �����

	output = "";
	output += make_feature("HKIB-20000_005.txt");//5������ �б�

	outFile.open("test_feature.txt");
	outFile << output;
	outFile.close();

	sort_remove_overlap("test_feature.txt", "test_feature_sort_remove.txt");
	reconstruct_chi("test_feature_sort_remove.txt", "test_feature_reconstruct.txt");
	feature_to_chi("test_feature_reconstruct.txt", "test_replace_chi.txt");
	final_sort("test_replace_chi.txt", "test_chi_final.txt");
	make_chi_study("test_chi_final.txt", "test_chi_studying.txt");
}


/*���� ���� �� string�� ����, ī�װ� ��ȣ, ���� ��ȣ �������� �����Ͽ� ����*/
string make_feature(string file_name) {
	ifstream inFile;
	char a[4000];
	int cat = 0;
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
					if (strcmp(feature, "�ǰ��� ����") == 0) {
						cat = 1;
						cat0++;
						break;
					}
					else if (strcmp(feature, "����") == 0) {
						cat = 2;
						cat1++;
						break;
					}
					else if (strcmp(feature, "����") == 0) {
						cat = 3;
						cat2++;
						break;
					}
					else if (strcmp(feature, "����") == 0) {
						cat = 4;
						cat3++;
						break;
					}
					else if (strcmp(feature, "��ȭ�� ����") == 0) {
						cat = 5;
						cat4++;
						break;
					}
					else if (strcmp(feature, "��ȸ") == 0) {
						cat = 6;
						cat5++;
						break;
					}
					else if (strcmp(feature, "���") == 0) {
						cat = 7;
						cat6++;
						break;
					}
					else if (strcmp(feature, "������Ȱ") == 0) {
						cat = 8;
						cat7++;
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
				output += cat + '0';
				output += " ";
				output += to_string(b);
				output += "\n";

				feature = strtok_s(NULL, token, &context);
			}
		}
	}
	inFile.close();

	return output;
}

/*
���� ������ �ҷ��� �� �ߺ����� ���� �� �� ���Ϸ� �ٽ� ����
*/
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

/*
�ߺ��� ���ŵ� ������ �̿��ؼ� ī�̽���� ����ϰ� ������������ �ε����� �ް� ���Ϸ� ����
*/
void cal_chi(string file_name, string result_name) {
	ifstream inFile;
	ofstream outFile;
	int cnt = 0, first = 0, k = 0; 
	double total = 15978.0;
	double cat_total[8] = { 348.0, 5023.0, 539.0, 479.0, 2362.0, 3637.0, 3309.0, 281.0 };//�� ī�װ� ����
	double cat_num[8] = { 0.0, };
	double chi[8] = { 0.0, };
	double A = 0.0, B = 0.0, C = 0.0, D = 0.0;
	char temp[600];
	string out, result[600000];
	char* feature = NULL;
	char *context = NULL;
	char token[] = " ";
	
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
					chi[i] = total * (((A * D) - (C * B)) * ((A * D) - (C * B)))  * (1.0 / (A + B)) * (1.0/(B + D)) * (1.0/(A + C)) * (1.0/(C + D));
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
	outFile.open(result_name);
	int j = 1;
	while (i >= 0) {
		outFile << j++ << ":";
		outFile << result[i--] << endl;
	}
	outFile.close();
}

/*
ī�̽����� ������ ������ ����
*/
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

/*
reconstruct_chi���� ������ ������ ������ �� ���Ϸ� ����
*/
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

/*
����� �ܾ���� ī�̽����� ������ ġȯ
*/
void feature_to_chi(string file_name1, string result_name) {
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
		while(first <= last) {
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

/*
ġȯ�� ī�̽����� ������ ������������ ����
*/
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

/*
SVM�� �� �������� ������ �����Ͽ� ����
*/
void make_chi_study(string file_name, string result_name) {
	ifstream inFile;
	ofstream outFile;
	string out[16000],s[3], t, before;
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
	cout << cnt<< endl;
	inFile.close();


	//outFile.open("chi_studying.txt");
	outFile.open(result_name);
	i = 0;
	while (i <= cnt) {
		outFile << out[i++] << endl;
	}
	outFile.close();

}

/*
��Ʈ�� �ʿ��� ���Լ� 1
*/
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

/*
��Ʈ�� �ʿ��� ���Լ� 2
*/
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

/*
ī�̽����� �ִ밪
*/
double max(double* chi) {
	double max = chi[0];
	for (int i = 0; i < 8; i++) {
		if (max < chi[i])
			max = chi[i];
	}
	return max;
}

void abc() {
	ifstream inFile;
	ofstream outFile;
	int a[2][4022] = { 0, };
	char *feat = NULL, *context = NULL;
	char temp[1000000];
	int arr[8][8] = { 0, };
	int cnt = 0, i =0;

	inFile.open("output.txt");
	if (inFile.is_open()) {
		while (inFile.getline(temp, sizeof(temp))) {
			a[0][cnt++] = atoi(temp);
		}
	}
	inFile.close();
	
	cout << cnt << endl;
	cnt = 0;
	inFile.open("test_chi_studying.txt");
	if (inFile.is_open()) {
		while (inFile.getline(temp, sizeof(temp))) {
			a[1][cnt++] = atoi(strtok_s(temp, " ", &context));
		}
	}
	inFile.close();
	cout << cnt << endl;
	
	while (i < 4022) {
		int t = a[0][i], k = a[1][i];
		if (t == k) {
			arr[t - 1][t - 1]++;
		}
		else {
			arr[t - 1][k - 1]++;
		}
		i++;
	}

	
	int total = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << arr[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
	cout << "101\t1235\t141\t104\t601\t883\t882\t75" << endl;
}