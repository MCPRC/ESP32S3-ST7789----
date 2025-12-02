#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
GB2312到Unicode映射表生成工具
用于生成完整的GB2312字符集到Unicode的映射表，格式化为C++代码
"""

def generate_gb2312_map():
    """生成GB2312到Unicode的映射表"""
    gb2312_to_unicode = []
    
    # GB2312字符集范围：
    # 区码：0xA1 - 0xFE (161-254)
    # 位码：0xA1 - 0xFE (161-254)
    # 共94个区，每个区94个位
    
    for area in range(0xA1, 0xFF):
        for pos in range(0xA1, 0xFF):
            # 组合成GB2312编码
            gb2312 = (area << 8) | pos
            
            try:
                # 转换为Unicode
                # 先构造GB2312字节序列
                gb2312_bytes = bytes([area, pos])
                # 解码为Unicode字符串
                unicode_str = gb2312_bytes.decode('gb2312')
                # 获取Unicode码点
                unicode_code = ord(unicode_str)
                
                # 添加到映射表
                gb2312_to_unicode.append((gb2312, unicode_code))
            except UnicodeDecodeError:
                # 跳过无法解码的字符
                continue
    
    return gb2312_to_unicode

def format_as_cpp_code(gb2312_to_unicode):
    """将映射表格式化为C++代码"""
    # 按Unicode码点排序，方便二分查找
    gb2312_to_unicode.sort(key=lambda x: x[1])
    
    # 开始生成代码
    cpp_code = """// GB2312到Unicode映射表（自动生成）
// 生成时间：2025-12-02
// 总字符数: """
    
    # 添加字符数
    cpp_code += str(len(gb2312_to_unicode))
    
    # 继续生成代码
    cpp_code += """

// 映射表结构：
// struct GB2312MapItem {
//   uint32_t unicode;  // Unicode码点
//   uint16_t gb2312;   // GB2312编码
// };

static const struct GB2312MapItem {
  uint32_t unicode;
  uint16_t gb2312;
} gb2312_map[] = {
"""
    
    # 生成映射表项
    for gb2312, unicode in gb2312_to_unicode:
        # 使用f-string，避免format()的大括号冲突
        cpp_code += f"  {{0x{unicode:04X}, 0x{gb2312:04X}}}, // {chr(unicode)}\n"
    
    # 结束映射表
    cpp_code += """};

// 二分查找GB2312编码
inline uint16_t unicode_to_gb2312(uint32_t unicode) {
  int left = 0;
  int right = sizeof(gb2312_map) / sizeof(gb2312_map[0]) - 1;
  
  while (left <= right) {
    int mid = left + (right - left) / 2;
    
    if (gb2312_map[mid].unicode == unicode) {
      return gb2312_map[mid].gb2312;
    } else if (gb2312_map[mid].unicode < unicode) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  
  return 0; // 未找到
}
"""
    
    return cpp_code

def main():
    """主函数"""
    print("正在生成GB2312到Unicode映射表...")
    
    # 生成映射表
    gb2312_to_unicode = generate_gb2312_map()
    
    # 格式化为C++代码
    cpp_code = format_as_cpp_code(gb2312_to_unicode)
    
    # 保存到文件
    with open("gb2312_map.h", "w", encoding="utf-8") as f:
        f.write(cpp_code)
    
    print(f"映射表生成完成！")
    print(f"总字符数：{len(gb2312_to_unicode)}")
    print(f"文件保存为：gb2312_map.h")
    print(f"文件大小：约{len(cpp_code) / 1024:.1f}KB")

if __name__ == "__main__":
    main()
