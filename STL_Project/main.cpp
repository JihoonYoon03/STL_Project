#include <iostream>
#include <string>
#include <memory>
#include <array>
#include <fstream>
#include <algorithm>
#include <filesystem>

/*
[조건]
e-class에서 받은 "2026 STL 과제 자료생성.exe" 파일을 받아 실행한다.
- 파일은 x64, Release mode로 빌드하였다.

폴더에 "2026 STL 과제 파일 - 학번"이 생성되었나 확인한다.
- 파일에는 300'0000(3백만)개의 Player 객체가 기록되어 있다.

파일은 바이너리(ios::binary)로 열었으며
다음과 같은 멤버함수 write를 사용하여 300만개의 Player 객체를 기록하였다.

void Player::write( ostream& os ) {
	os.write((char*)this, sizeof(Player));
	os.write((char*)p.get(), num);
}

* 주의 - 이 과제에서 Player 객체는 추가되거나 삭제되지 않는다.
 - 파일을 한 번만 읽고 과제를 해결하여야 한다.(메모리에 저장 후 해결)
 - 문제를 순서대로 해결하여야 한다

[과제]

2.	점수가 가장 큰 Player를 찾아 화면에 출력하라.(동점 모두 출력)
	Player의 평균 점수를 계산하여 화면에 출력하라.
	- 어떻게 찾고 계산하였는지 보고서에 설명하라.

3.	id가 서로 같은 객체를 찾아 "같은아이디.txt"에 기록하라.
	id가 같은 객체는 모두 몇 개인지 화면에 출력하라.
	파일에는 id가 같은 Player 객체의 이름과 아이디를 한 줄 씩 기록한다.
	- 어떻게 같은 id를 찾았는지 보고서에 설명하라.

4.	Player의 멤버 p가 가리키는 메모리에는 파일에서 읽은 num개의 char가 저장되어 있어야 한다.
	메모리에 저장된 char를 오름차순으로 정렬하라.
	'0'부터 ‘9’까지 모든 숫자가 있는 Player를 찾아 모두 몇 객체인지 출력하라. - 어떻게 찾았는지 보고서에 설명하라.

5. [ LOOP ] id를 입력받아 존재하는 id라면 다음 내용을 한 번에 화면 출력하라.
	- 모든 Player가 id 기준 오름차순으로 정렬되어 있는 상태에서
	해당 id 포함 앞과 뒤 Player의 정보를 출력한다.
	id가 같은 Player가 둘 이상이면 이들의 정보를 모두 출력하여야 한다.
	
	- Player가 name 기준 오름차순으로 정렬되어 있는 상태에서
	해당 name 포함 앞과 뒤 Player의 정보를 출력한다.
	같은 name이 여럿일 경우 바로 앞뒤의 Player 정보만 출력하면 된다.
	
	- Player가 score 기준 오름차순으로 정렬되어 있는 상태에서
	해당 score 포함 앞과 뒤 Player의 정보를 출력한다.
	같은 score가 여럿일 경우 바로 앞뒤 한명의 Player 정보만 출력하면 된다.

	* 프로그램은 5번을 무한히 반복할 수 있어야 한다.

	- 5번 문제를 어떻게 해결하였는지 보고서에 설명하라.
*/

class Player {
public:
	void read( std::ifstream& in ) {
		Player temp;
		in.read((char*)&temp, sizeof(Player));

		name = temp.name.c_str();
		score = temp.score;
		id = temp.id;
		num = temp.num;
		temp.p.release();

		p = std::make_unique<char[]>(num);
		in.read(p.get(), num);
	}

private:
	std::string name; // 이름, 길이[3, 15], ['a', 'z']로만 구성
	int score; // 점수, 정규분포
	size_t id; // 아이디, 겹치는 아이디 있을 수 있음
	size_t num; // free store에 확보한 바이트 수
	std::unique_ptr<char[]> p; // free store에 확보한 메모리
};

std::array<Player, 300'0000> players;

int main()
{
	/*
	1.	파일에 저장한 모든 Player 정보를 읽어 컨테이너에 저장하라.
		제일 마지막 Player의 정보를 다음과 같은 형식으로 화면에 출력하라.
	*/
	std::ifstream in{ "2026 STL 과제 파일 - 2022184025", std::ios::binary };

	if (not in) {
		std::cout << "파일 열기 실패" << std::endl;
		return 2022184025;
	}
		
	int cnt = 0;
	for (auto& p : players) {
		p.read(in);
		++cnt;
	}

	std::cout << "객체 개수: " << cnt << std::endl;

	return 0;
}