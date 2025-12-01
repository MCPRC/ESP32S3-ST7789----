#include "core/page_manager.h"

std::vector<Page*> PageManager::_pages;
int PageManager::_currentPageIndex = -1;
Page* PageManager::_currentPage = nullptr;

void PageManager::init() {
  _pages.clear();
  _currentPageIndex = -1;
  _currentPage = nullptr;
}

void PageManager::addPage(Page* page) {
  _pages.push_back(page);
  page->init();
}

void PageManager::setCurrentPage(int pageIndex) {
  if (pageIndex < 0 || pageIndex >= _pages.size()) {
    return;
  }
  
  if (_currentPage != nullptr) {
    _currentPage->onExit();
  }
  
  _currentPageIndex = pageIndex;
  _currentPage = _pages[pageIndex];
  _currentPage->onEnter();
}

void PageManager::update() {
  if (_currentPage != nullptr) {
    _currentPage->update();
  }
}

void PageManager::draw(uint16_t *fb) {
  if (_currentPage != nullptr) {
    _currentPage->draw(fb);
  }
}
