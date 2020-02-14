#pragma once
class Input
{

private:
	struct Mouse {
		int x, y;
		bool left;
		bool right;
	};
public:
	void setKeyDown(int key);
	void setKeyUp(int key);
	bool isKeyDown(int key);

	void setMouseX(int lx);
	void setMouseY(int ly);
	void setMousePosition(int lx, int ly);
	void setMouseLeftDown(bool is);
	void setMouseRightDown(bool is);
	bool isTherePlayerInputWSAD();
	bool isTherePlayerInputQE();
	int getMouseX();
	int getMouseY();
	bool getMouseLeftDown();
	bool getMouseRightDown();
	
	Input();
	~Input();

private:
	bool keys[256] {false};
	Mouse mouse;
};

