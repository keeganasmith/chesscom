mac_test:
	g++ -std=c++17 -I/opt/homebrew/opt/openssl/include -I./include -I/opt/homebrew/opt/asio/include -L/opt/homebrew/opt/openssl/lib -lssl -lcrypto test.cpp ./chess_api/chesscom_api.cpp ./common/helper.cpp ./analysis/Stockfish_Wrapper.cpp
windass:
	g++ -std=c++17 main.cpp ./chess_api/chesscom_api.cpp ./common/helper.cpp ./analysis/Stockfish_Wrapper.cpp -lssl -lcrypto
mac_app:
	g++ -std=c++17 -I/opt/homebrew/opt/openssl/include -I./include -I/opt/homebrew/opt/asio/include -L/opt/homebrew/opt/openssl/lib -lssl -lcrypto main.cpp ./chess_api/chesscom_api.cpp ./common/helper.cpp ./analysis/Stockfish_Wrapper.cpp
