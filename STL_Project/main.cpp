#include <iostream>
#include <string>
#include <memory>
#include <array>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <print>

/*
[Á¶°Ç]
e-class¿¡¼­ ¹ÞÀº "2026 STL °úÁ¦ ÀÚ·á»ý¼º.exe" ÆÄÀÏÀ» ¹Þ¾Æ ½ÇÇàÇÑ´Ù.
- ÆÄÀÏÀº x64, Release mode·Î ºôµåÇÏ¿´´Ù.

Æú´õ¿¡ "2026 STL °úÁ¦ ÆÄÀÏ - ÇÐ¹ø"ÀÌ »ý¼ºµÇ¾ú³ª È®ÀÎÇÑ´Ù.
- ÆÄÀÏ¿¡´Â 300'0000(3¹é¸¸)°³ÀÇ Player °´Ã¼°¡ ±â·ÏµÇ¾î ÀÖ´Ù.

ÆÄÀÏÀº ¹ÙÀÌ³Ê¸®(ios::binary)·Î ¿­¾úÀ¸¸ç
´ÙÀ½°ú °°Àº ¸â¹öÇÔ¼ö write¸¦ »ç¿ëÇÏ¿© 300¸¸°³ÀÇ Player °´Ã¼¸¦ ±â·ÏÇÏ¿´´Ù.

void Player::write( ostream& os ) {
	os.write((char*)this, sizeof(Player));
	os.write((char*)p.get(), num);
}

* ÁÖÀÇ - ÀÌ °úÁ¦¿¡¼­ Player °´Ã¼´Â Ãß°¡µÇ°Å³ª »èÁ¦µÇÁö ¾Ê´Â´Ù.
 - ÆÄÀÏÀ» ÇÑ ¹ø¸¸ ÀÐ°í °úÁ¦¸¦ ÇØ°áÇÏ¿©¾ß ÇÑ´Ù.(¸Þ¸ð¸®¿¡ ÀúÀå ÈÄ ÇØ°á)
 - ¹®Á¦¸¦ ¼ø¼­´ë·Î ÇØ°áÇÏ¿©¾ß ÇÑ´Ù

*/

class Player {
public:
	void read( std::ifstream& in ) {
		Player temp;
		in.read((char*)(&temp), sizeof(Player));
		
		name = temp.name.c_str();
		score = temp.score;
		id = temp.id;
		num = temp.num;
		temp.p.release();

		p = std::make_unique<char[]>(num);
		in.read(p.get(), num);
	}

	friend std::ostream& operator<<(std::ostream& os, const Player& player) {
		std::print("ÀÌ¸§: {:16}, ¾ÆÀÌµð: {}, Á¡¼ö: {}, ÀÚ¿ø¼ö: {}, ÀúÀåµÈ ±ÛÀÚ: {}", player.name, player.id, player.score, player.num, player.p.get());
		return os;
	}

	std::string& getName() {
		return name;
	}

	int getScore() const {
		return score;
	}

	size_t getID() const {
		return id;
	}

	size_t size() const {
		return num;
	}

	char* data() const {
		return p.get();
	}

	std::string info() const {
		std::string inf = "ÀÌ¸§: " + name + ", ID: " + std::to_string(id);
		return inf;
	}

private:
	std::string name; // ÀÌ¸§, ±æÀÌ[3, 15], ['a', 'z']·Î¸¸ ±¸¼º
	int score; // Á¡¼ö, Á¤±ÔºÐÆ÷
	size_t id; // ¾ÆÀÌµð, °ãÄ¡´Â ¾ÆÀÌµð ÀÖÀ» ¼ö ÀÖÀ½
	size_t num; // free store¿¡ È®º¸ÇÑ ¹ÙÀÌÆ® ¼ö
	std::unique_ptr<char[]> p; // free store¿¡ È®º¸ÇÑ ¸Þ¸ð¸®
};

std::array<Player, 300'0000> players;
std::array<unsigned int, 300'0000> indexName;
std::array<unsigned int, 300'0000> indexScore;

int main()
{
	/*
	1.	ÆÄÀÏ¿¡ ÀúÀåÇÑ ¸ðµç Player Á¤º¸¸¦ ÀÐ¾î ÄÁÅ×ÀÌ³Ê¿¡ ÀúÀåÇÏ¶ó.
		Á¦ÀÏ ¸¶Áö¸· PlayerÀÇ Á¤º¸¸¦ ´ÙÀ½°ú °°Àº Çü½ÄÀ¸·Î È­¸é¿¡ Ãâ·ÂÇÏ¶ó.
	*/
	std::ifstream in{ "2026 STL °úÁ¦ ÆÄÀÏ - 2022184025", std::ios::binary };

	if (not in) {
		std::cout << "ÆÄÀÏ ¿­±â ½ÇÆÐ" << std::endl;
		return 2022184025;
	}

	for (auto& p : players) {
		p.read(in);
	}

	std::cout << players.back() << std::endl << std::endl;

	/*
	2.	Á¡¼ö°¡ °¡Àå Å« Player¸¦ Ã£¾Æ È­¸é¿¡ Ãâ·ÂÇÏ¶ó.(µ¿Á¡ ¸ðµÎ Ãâ·Â)
		PlayerÀÇ Æò±Õ Á¡¼ö¸¦ °è»êÇÏ¿© È­¸é¿¡ Ãâ·ÂÇÏ¶ó.
		- ¾î¶»°Ô Ã£°í °è»êÇÏ¿´´ÂÁö º¸°í¼­¿¡ ¼³¸íÇÏ¶ó.
	*/

	{
		long long sum{};
		int curVal{};
		int maxVal = std::numeric_limits<int>::min();
		std::for_each(players.begin(), players.end(), [&](const Player& player) {
			curVal = player.getScore();
			sum += curVal;
			if (curVal > maxVal)
				maxVal = curVal;
			});

		for (const Player& player : players) {
			if (player.getScore() == maxVal)
				std::cout << player << std::endl;
		}

		double average = static_cast<double>(sum) / players.size();

		std::cout << std::endl;
		std::print("Æò±Õ Á¡¼ö: {:f}", average);
		std::cout << std::endl;
	}
	/*
	3.	id°¡ ¼­·Î °°Àº °´Ã¼¸¦ Ã£¾Æ "°°Àº¾ÆÀÌµð.txt"¿¡ ±â·ÏÇÏ¶ó.
		id°¡ °°Àº °´Ã¼´Â ¸ðµÎ ¸î °³ÀÎÁö È­¸é¿¡ Ãâ·ÂÇÏ¶ó.
		ÆÄÀÏ¿¡´Â id°¡ °°Àº Player °´Ã¼ÀÇ ÀÌ¸§°ú ¾ÆÀÌµð¸¦ ÇÑ ÁÙ ¾¿ ±â·ÏÇÑ´Ù.
		- ¾î¶»°Ô °°Àº id¸¦ Ã£¾Ò´ÂÁö º¸°í¼­¿¡ ¼³¸íÇÏ¶ó.
	*/
	{
		std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
			return a.getID() < b.getID();
			});

		std::ofstream out{ "°°Àº¾ÆÀÌµð.txt" };

		size_t count{};

		auto rangeBegin = players.begin();

		while (true) {
			rangeBegin = std::adjacent_find(rangeBegin, players.end(), [](const Player& a, const Player& b) {
				return a.getID() == b.getID();
				});

			if (rangeBegin == players.end())
				break;

			size_t curID = rangeBegin->getID();

			auto rangeEnd = std::upper_bound(rangeBegin, players.end(), curID, [](size_t id, const Player& player) {
				return id < player.getID();
				});

			for (auto cur = rangeBegin; cur != rangeEnd; ++cur) {
				out << cur->info() + "\n";
				count++;
			}

			rangeBegin = rangeEnd;
		}

		std::cout << "°°Àº ¾ÆÀÌµð °´Ã¼ °³¼ö: " << count << std::endl;
	}
	/*
	4.	PlayerÀÇ ¸â¹ö p°¡ °¡¸®Å°´Â ¸Þ¸ð¸®¿¡´Â ÆÄÀÏ¿¡¼­ ÀÐÀº num°³ÀÇ char°¡ ÀúÀåµÇ¾î ÀÖ¾î¾ß ÇÑ´Ù.
		¸Þ¸ð¸®¿¡ ÀúÀåµÈ char¸¦ ¿À¸§Â÷¼øÀ¸·Î Á¤·ÄÇÏ¶ó.
		'0'ºÎÅÍ '9'±îÁö ¸ðµç ¼ýÀÚ°¡ ÀÖ´Â Player¸¦ Ã£¾Æ ¸ðµÎ ¸î °´Ã¼ÀÎÁö Ãâ·ÂÇÏ¶ó. - ¾î¶»°Ô Ã£¾Ò´ÂÁö º¸°í¼­¿¡ ¼³¸íÇÏ¶ó.
	*/
	{
		for (Player& player : players) {
			//std::sort(player.data(), player.data() + player.size());
		}

		std::string digits{ "0123456789" };
		size_t count = std::count_if(players.begin(), players.end(), [&](const Player& player) {
			return std::includes(player.data(), player.data() + player.size(), digits.begin(), digits.end());
			});

		std::cout << "0ºÎÅÍ 9±îÁöÀÇ ¼ýÀÚ¸¦ ÀúÀåÇÏ´Â Player °´Ã¼ °³¼ö: " << count << std::endl;
	}

	/*
	5. [ LOOP ] id¸¦ ÀÔ·Â¹Þ¾Æ Á¸ÀçÇÏ´Â id¶ó¸é ´ÙÀ½ ³»¿ëÀ» ÇÑ ¹ø¿¡ È­¸é Ãâ·ÂÇÏ¶ó.
		- ¸ðµç Player°¡ id ±âÁØ ¿À¸§Â÷¼øÀ¸·Î Á¤·ÄµÇ¾î ÀÖ´Â »óÅÂ¿¡¼­
		ÇØ´ç id Æ÷ÇÔ ¾Õ°ú µÚ PlayerÀÇ Á¤º¸¸¦ Ãâ·ÂÇÑ´Ù.
		id°¡ °°Àº Player°¡ µÑ ÀÌ»óÀÌ¸é ÀÌµéÀÇ Á¤º¸¸¦ ¸ðµÎ Ãâ·ÂÇÏ¿©¾ß ÇÑ´Ù.
	
		- Player°¡ name ±âÁØ ¿À¸§Â÷¼øÀ¸·Î Á¤·ÄµÇ¾î ÀÖ´Â »óÅÂ¿¡¼­
		ÇØ´ç name Æ÷ÇÔ ¾Õ°ú µÚ PlayerÀÇ Á¤º¸¸¦ Ãâ·ÂÇÑ´Ù.
		°°Àº nameÀÌ ¿©·µÀÏ °æ¿ì ¹Ù·Î ¾ÕµÚÀÇ Player Á¤º¸¸¸ Ãâ·ÂÇÏ¸é µÈ´Ù.
	
		- Player°¡ score ±âÁØ ¿À¸§Â÷¼øÀ¸·Î Á¤·ÄµÇ¾î ÀÖ´Â »óÅÂ¿¡¼­
		ÇØ´ç score Æ÷ÇÔ ¾Õ°ú µÚ PlayerÀÇ Á¤º¸¸¦ Ãâ·ÂÇÑ´Ù.
		°°Àº score°¡ ¿©·µÀÏ °æ¿ì ¹Ù·Î ¾ÕµÚ ÇÑ¸íÀÇ Player Á¤º¸¸¸ Ãâ·ÂÇÏ¸é µÈ´Ù.

		* ÇÁ·Î±×·¥Àº 5¹øÀ» ¹«ÇÑÈ÷ ¹Ýº¹ÇÒ ¼ö ÀÖ¾î¾ß ÇÑ´Ù.

		- 5¹ø ¹®Á¦¸¦ ¾î¶»°Ô ÇØ°áÇÏ¿´´ÂÁö º¸°í¼­¿¡ ¼³¸íÇÏ¶ó.
	*/
	{
		// ID·Î´Â ÀÌ¹Ì Á¤·ÄµÈ »óÅÂÀÌ¹Ç·Î name°ú score¸¸ Á¤·Ä Á¤º¸ ÀúÀå
		std::iota(indexName.begin(), indexName.end(), 0U);
		indexScore = indexName;

		std::sort(indexName.begin(), indexName.end(), [&](unsigned int a, unsigned int b) {
			return players[a].getName() < players[b].getName();
			});

		std::sort(indexScore.begin(), indexScore.end(), [&](unsigned int a, unsigned int b) {
			return players[a].getScore() < players[b].getScore();
			});


		std::string input;
		while (true) {
			std::cout << "°Ë»öÇÒ ID ÀÔ·Â: ";
			if (!(std::cin >> input)) {
				std::cout << "ÀÔ·Â Á¾·á" << std::endl;
				break;
			}

			try {
				// ¼ýÀÚ°¡ ¸î °³ ÀúÀåµÇ¾ú´Â°¡¸¦ ³ªÅ¸³»´Â pos
				size_t pos;
				size_t ID = std::stoull(input, &pos);

				// pos != input.size() -> ¹®ÀÚ°¡ ÀÖ´Â °æ¿ì
				if (input.front() == '-' || pos != input.size())
					throw std::invalid_argument("Àß¸øµÈ ÀÔ·Â °ª");

				// ID Ãâ·Â -----------------------------------------------------------------------------------
				auto rangeBeginID = std::find_if(players.begin(), players.end(), [&](const Player& player) {
					return player.getID() == ID;
					});
				if (rangeBeginID == players.end()) {
					std::cout << "Á¸ÀçÇÏÁö ¾Ê´Â IDÀÔ´Ï´Ù." << std::endl;
					continue;
				}

				auto rangeEndID = std::upper_bound(players.begin(), players.end(), ID, [](size_t id, const Player& player) {
					return id < player.getID();
					});


				std::cout		<< "¦£¦¡ ID Á¤·Ä ±âÁØ ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤\n\n";
				if (rangeBeginID != players.begin()) {
					std::cout	<< "¦§¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ ¾ÕÂÊ Player ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦©\n"
								<< *(rangeBeginID - 1) << std::endl << std::endl;
				}

				std::cout		<< "¦§¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ µ¿ÀÏ IDÀÎ Player ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦©\n";
				for (auto i = rangeBeginID; i != rangeEndID; ++i) {
					std::cout	<< *i << std::endl << std::endl;
				}
				std::cout << std::endl;

				if (rangeEndID != players.end()) {
					std::cout	<< "¦§¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ µÚÂÊ Player ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦©\n"
								<< *rangeEndID << std::endl << std::endl;
				}
				std::cout		<< "¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥\n\n";

				// name, score´Â º»ÀÎ Æ÷ÇÔ ¾ÕµÚ Ãâ·ÂÇÏ¸é µÇ¹Ç·Î Begin¸¸ ±¸ÇÑ´Ù.
				auto iterName = std::find_if(indexName.begin(), indexName.end(), [&](unsigned int i) {
					return players[i].getID() == ID;
					});

				auto iterScore = std::find_if(indexScore.begin(), indexScore.end(), [&](unsigned int i) {
					return players[i].getID() == ID;
					});

				// name Ãâ·Â --------------------------------------------------------------------------
				std::cout		<< "¦£¦¡ ÀÌ¸§ Á¤·Ä ±âÁØ ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤\n\n";
				if (iterName != indexName.begin()) {
					std::cout	<< "¦§¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ ¾ÕÂÊ Player ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦©\n"
								<< players[*(iterName - 1)] << std::endl << std::endl;
				}

				std::cout		<< "¦§¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ ÇØ´ç IDÀÎ Player ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦©\n";
				std::cout		<< players[*iterName] << std::endl << std::endl;

				if (iterName + 1 != indexName.end()) {
					std::cout	<< "¦§¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ µÚÂÊ Player ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦©\n"
								<< players[*(iterName + 1)] << std::endl << std::endl;
				}
				std::cout		<< "¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥\n\n";

				// score Ãâ·Â --------------------------------------------------------------------------
				std::cout		<< "¦£¦¡ Á¡¼ö Á¤·Ä ±âÁØ ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤\n\n";
				if (iterScore != indexScore.begin()) {
					std::cout	<< "¦§¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ ¾ÕÂÊ Player ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦©\n"
								<< players[*(iterScore - 1)] << std::endl << std::endl;
				}

				std::cout		<< "¦§¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ ÇØ´ç IDÀÎ Player ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦©\n";
				std::cout		<< players[*iterScore] << std::endl << std::endl;

				if (iterScore + 1 != indexScore.end()) {
					std::cout	<< "¦§¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ µÚÂÊ Player ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦©\n"
								<< players[*(iterScore + 1)] << std::endl << std::endl;
				}
				std::cout		<< "¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥\n\n";
			}
			catch (std::exception& e) {
				std::cout << "Àß¸øµÈ ÀÔ·ÂÀÔ´Ï´Ù." << std::endl;
			}

		}
	}

	return 0;
}