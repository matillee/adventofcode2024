/**
 * @file file_formatter_test.cpp
 * @brief Unit tests for the FileFormatter class and disk fragmenter logic.
 *
 * These tests verify correct parsing, rearrangement, and checksum calculation for various disk map scenarios.
 */
#include "gtest/gtest.h"
// #include "gmock/gmock.h"
#include <sstream>
#include "file_formatter.hpp"

/**
 * @test OneTimeParsedDiskMap
 * @brief Tests parsing of the original disk map into the one-time parsed disk map.
 */
TEST(DiskFragmenterTest, OneTimeParsedDiskMap)
{   
    std::vector<char> original_diskmap = {'2','3','3','3','1','3','3','1','2','1','4','1','4','1','3','1','4','0','2'};
    FileFormatter disk_fragmenter(original_diskmap, false);
    EXPECT_EQ(disk_fragmenter.print_diskmap_order(), std::vector<std::string>({"(0)","(0)",".",".",".","(1)","(1)","(1)",".",".",".","(2)",".",".",".","(3)","(3)","(3)",".","(4)","(4)",".","(5)","(5)","(5)","(5)",".","(6)","(6)","(6)","(6)",".","(7)","(7)","(7)",".","(8)","(8)","(8)","(8)","(9)","(9)"}));
}

/**
 * @test RearrangedDiskMapFragmentationAllowed
 * @brief Tests rearrangement of the disk map when fragmentation is allowed.
 */
TEST(DiskFragmenterTest, RearrangedDiskMapFragmentationAllowed)
{
    std::vector<char> original_diskmap = {'2','3','3','3','1','3','3','1','2','1','4','1','4','1','3','1','4','0','2'};
    FileFormatter disk_fragmenter(original_diskmap, true);
    disk_fragmenter.get_checksum();
    EXPECT_EQ(disk_fragmenter.print_diskmap_order(), std::vector<std::string>({"(0)","(0)","(9)","(9)","(8)","(1)","(1)","(1)","(8)","(8)","(8)","(2)","(7)","(7)","(7)","(3)","(3)","(3)","(6)","(4)","(4)","(6)","(5)","(5)","(5)","(5)","(6)","(6)",".",".",".",".",".",".",".",".",".",".",".",".",".","."}));
}

/**
 * @test ChecksumFragmentationAllowed
 * @brief Tests checksum calculation when fragmentation is allowed.
 */
TEST(DiskFragmenterTest, ChecksumFragmentationAllowed)
{
    std::vector<char> original_diskmap = {'2','3','3','3','1','3','3','1','2','1','4','1','4','1','3','1','4','0','2'};
    FileFormatter disk_fragmenter(original_diskmap, true);
    EXPECT_EQ(disk_fragmenter.get_checksum(), 1928);
}

/**
 * @test RearrangedDiskMap
 * @brief Tests rearrangement of the disk map when fragmentation is not allowed.
 */
TEST(DiskFragmenterTest, RearrangedDiskMap)
{
    std::vector<char> original_diskmap = {'2','3','3','3','1','3','3','1','2','1','4','1','4','1','3','1','4','0','2'};
    FileFormatter disk_fragmenter(original_diskmap, false);
    disk_fragmenter.get_checksum();
    EXPECT_EQ(disk_fragmenter.print_diskmap_order(), std::vector<std::string>({"(0)","(0)","(9)","(9)","(2)","(1)","(1)","(1)","(7)","(7)","(7)",".","(4)","(4)",".","(3)","(3)","(3)",".",".",".",".","(5)","(5)","(5)","(5)",".","(6)","(6)","(6)","(6)",".",".",".",".",".","(8)","(8)","(8)","(8)",".","."}));
}

/**
 * @test Checksum
 * @brief Tests checksum calculation when fragmentation is not allowed.
 */
TEST(DiskFragmenterTest, Checksum)
{
    std::vector<char> original_diskmap = {'2','3','3','3','1','3','3','1','2','1','4','1','4','1','3','1','4','0','2'};
    FileFormatter disk_fragmenter(original_diskmap, false);
    EXPECT_EQ(disk_fragmenter.get_checksum(), 2858);
}