#pragma once

//сортировка по дате //по убыванию

bool compareTimeLess(data_t first, data_t second)
{
  if(*(first._year)!=*(second._year))
  {
    return *(first._year)>*(second._year);
  }
  else
  {
    if(*(first._month)!=*(second._month))
    {
      return *(first._month)>*(second._month);
    }
    else
    {
      if(*(first._day)!=*(second._day))
      {
        return *(first._day)>*(second._day);
      }
      else
      {
        if(*(first._hours)!=*(second._hours))
        {
          return *(first._hours)>*(second._hours);
        }
        else
        {
          if(*(first._minutes)!=*(second._minutes))
          {
            return *(first._minutes)>*(second._minutes);
          }
          else
          {
            return false;
          }
        }//hours==hours
      }//day==day
    }//month==month
  }//years==years
}


//сортировка по дате //возростание
bool compareTimeNotLess(data_t first, data_t second)
{
  if(*(first._year)!=*(second._year))
  {
    return *(first._year)<*(second._year);
  }
  else
  {
    if(*(first._month)!=*(second._month))
    {
      return *(first._month)<*(second._month);
    }
    else
    {
      if(*(first._day)!=*(second._day))
      {
        return *(first._day)<*(second._day);
      }
      else
      {
        if(*(first._hours)!=*(second._hours))
        {
          return *(first._hours)<*(second._hours);
        }
        else
        {
          if(*(first._minutes)!=*(second._minutes))
          {
            return *(first._minutes)<*(second._minutes);
          }
          else
          {
            return false;
          }
        }//hours==hours
      }//day==day
    }//month==month
  }//years==years
}


/*
//сортировка по размеру
bool compareSize(uintmax_t first, uintmax_t second)
{
  return first<second;
}

//сортировка по name
bool compareName(data first, data second)
{
  return lexicographical_compare(first._name.begin(),first._name.end(),second._name.begin(),second._name.end());
}

//сортировка по path
bool comparePath(data first, data second)
{
  return lexicographical_compare(first._path.begin(),first._path.end(),second._path.begin(),second._path.end());
}
*/