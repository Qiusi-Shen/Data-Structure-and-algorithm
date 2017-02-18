class File
{
public:
	File(string name);
	virtual ~File();
	virtual string name() const;
	virtual void open() const = 0;
	virtual void redisplay() const;

private:
	string m_name;
};

class TextMsg : public File
{
public:
	TextMsg(string name) : File(name){};
	virtual ~TextMsg();
	virtual void open() const;

private:

};

class Video : public File
{
public:
	Video(string name, int sec) : File(name),m_sec(sec) {};
	virtual ~Video();
	virtual void open() const;
	virtual void redisplay() const;

private:
	int m_sec;
};

class Picture : public File
{
public:
	Picture(string name) : File(name){};
	virtual ~Picture();
	virtual void open() const;

private:

};

Picture::~Picture()
{
	cout << "Destroying the pictrue " << File::name() << endl;
}

void Picture::open() const
{
	cout << "show picture";
}


Video::~Video()
{
	cout << "Destroying " << File::name() << ", a video" << endl;
}

void Video::redisplay() const
{
	cout << "replay video";
}

void Video::open() const
{
	cout << "play " << m_sec << " second video";
}

void TextMsg::open() const
{
	cout << "open text message";
}

TextMsg::~TextMsg()
{
	cout << "Destroying " << File::name() << ", a text message" << endl;
}

File::File(string name)
{
	m_name = name;
}

string File::name()const
{
	return m_name;
}
void File::redisplay() const
{
	cout<< "refresh the screen";
}

File::~File()
{
}
