#pragma once

using std::string;
using std::setw;
using std::strcpy;

class data_t
{
public:
  data_t(wstring creation=TEXT("empty"), wstring name=TEXT("empty"), wstring fpath=TEXT("empty"), wstring filesize=TEXT("empty"), uintmax_t fsize=0, int day=0, int month=0, int year=0, int hours=0, int minutes=0);
  data_t(const data_t&);//конструктор копирования
  
  const data_t& operator = (const data_t&);
  ~data_t();
  
  wstring _creation;
  
  wstring _name;
  wstring _path;
  wstring _filesize;

  
  uintmax_t _fsize;
  int *_day;
  int *_month;
  int *_year;
  int *_hours;
  int *_minutes;
};

data_t::data_t(wstring creation, wstring name, wstring fpath, wstring filesize , uintmax_t fsize, int day, int month, int year, int hours, int minutes)
  :_name(name),_path(fpath),_filesize(filesize),_creation(creation),_fsize(fsize)
{
  try
  {
  _day=new int(day);
  _month=new int(month);

  _year=new int(year);

  _hours=new int(hours);
  _minutes=new int(minutes);
  }
  catch (bad_alloc&)
  {
    MessageBox(NULL, TEXT("Error allocating memory"), TEXT("memory error"), MB_OK | MB_ICONERROR);
  }
}

data_t::data_t(const data_t& object)//конструктор копирования
{
  this->_creation=object._creation;
  this->_name=object._name;
  this->_path=object._path;
  this->_filesize=object._filesize;
  
  
  this->_fsize=object._fsize;

  try
  {
    this->_day=new int (*(object._day));
    this->_hours=new int (*(object._hours));
    this->_minutes=new int (*(object._minutes));
    this->_month=new int (*(object._month));
    this->_year=new int (*(object._year));
  }
  catch (bad_alloc&)
  {
    MessageBox(NULL, TEXT("Error allocating memory"), TEXT("memory error constructor"), MB_OK | MB_ICONERROR);
  }

}

//грузим оператор присваивания
const data_t& data_t::operator = (const data_t& object)
{
  this->_creation=object._creation;
  this->_name=object._name;
  this->_path=object._path;
  this->_filesize=object._filesize;
  
  
  this->_fsize=object._fsize;
  
  try
  {
    //_day operator = overload
    if(this->_day)//если существует
    {
      delete this->_day;//освобождаем память!
    }
    if(object._day)
    {
      this->_day=new int(*object._day);
    }
    else
    {
      this->_day=0;
    }
    
    //_month operator = overload
    if(this->_month)//если существует
    {
      delete this->_month;//освобождаем память!
    }
    if(object._month)
    {
      this->_month=new int(*object._month);
    }
    else
    {
      this->_month=0;
    }
    
    //_year operator = overload
    if(this->_year)//если существует
    {
      delete this->_year;//освобождаем память!
    }
    if(object._year)
    {
      this->_year=new int(*object._year);
    }
    else
    {
      this->_year=0;
    }
    
    //_minutes operator = overload
    if(this->_minutes)//если существует
    {
      delete this->_minutes;//освобождаем память!
    }
    if(object._minutes)
    {
      this->_minutes=new int(*object._minutes);
    }
    else
    {
      this->_minutes=0;
    }
    
    //_hours operator = overload
    if(this->_hours)//если существует
    {
      delete this->_hours;//освобождаем память!
    }
    if(object._hours)
    {
      this->_hours=new int(*object._hours);
    }
    else
    {
      this->_hours=0;
    }
  }
  catch (bad_alloc&)
  {
    MessageBox(NULL, TEXT("Error allocating memory"), TEXT("memory error operator ="), MB_OK | MB_ICONERROR);
  }

  return *this;
}

data_t::~data_t()
{
  delete _day;
  delete _month;
  delete _year;

  delete _hours;
  delete _minutes;
}



