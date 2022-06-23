
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>   
#include <windows.h>

using namespace std;


struct Prepare_data_for_polynom{
	int stepen;
	double x;
	vector<double> kf;
	double res;
};


DWORD WINAPI polynom(LPVOID par)
{
	Prepare_data_for_polynom* data = (Prepare_data_for_polynom*)par;
	double res = 0;
	for (int i = 0; i < data->stepen; i++)
	{
		res += data->kf[i] * pow(data->x, data->stepen - i);
		Sleep(15);
	}
	res += data->kf[data->stepen];
	Sleep(15);
	cout << "p(" << data->x << ") = " << res << endl;
	data->res = res;
	return 0;
}


void Prepare_data_for_main(Prepare_data_for_polynom& polynom) {
	cout << "Vvedite stepen' polynoma: ";
	cin >> polynom.stepen;
	for (int i = 0; i < polynom.stepen + 1; i++)
	{
		double kf;
		cout << "Koeficient: ";
		cin >> kf;
		polynom.kf.push_back(kf);
	}
	cout << endl;

}

int main() {
	Prepare_data_for_polynom num, denum;
	cout << " vvedite stepen i kf chislitelya" << endl;
	Prepare_data_for_main(num);
	cout << " vvedite stepen i kf znamenatelya" << endl;
	Prepare_data_for_main(denum);
	double x;
	cin >> x;
	cout << endl;
	num.x = x;
	denum.x = x;

	int kolichestvo_polynomov = 2;

	HANDLE* PolynomThreadHandls = new HANDLE[kolichestvo_polynomov];
	DWORD* PolynomThreadID = new DWORD[kolichestvo_polynomov];
	PolynomThreadHandls[0] = CreateThread(NULL, 0, polynom, (void*)&num, 0, &PolynomThreadID[0]);
	PolynomThreadHandls[1] = CreateThread(NULL, 0, polynom, (void*)&denum, 0, &PolynomThreadID[1]);
	WaitForMultipleObjects(kolichestvo_polynomov, PolynomThreadHandls, TRUE, INFINITE);

	cout << "f(" << x << ") = " << num.res / denum.res;

	return 0;
}