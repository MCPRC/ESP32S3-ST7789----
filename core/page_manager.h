#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <stdint.h>
#include <vector>

class Page {
public:
  virtual void init() = 0;
  virtual void update() = 0;
  virtual void draw(uint16_t *fb) = 0;
  virtual void onEnter() = 0;
  virtual void onExit() = 0;
};

class PageManager {
public:
  static void init();
  static void addPage(Page* page);
  static void setCurrentPage(int pageIndex);
  static void update();
  static void draw(uint16_t *fb);
  
private:
  static std::vector<Page*> _pages;
  static int _currentPageIndex;
  static Page* _currentPage;
};

#endif // PAGE_MANAGER_H
