#pragma once

#define MAX_BLOCK_SIZE (1UL << 14)

#define REQUIRED_BLOCKS(Size)                           \
  (((Size) + MAX_BLOCK_SIZE - 1) / MAX_BLOCK_SIZE)

#define BLOCK_SIZE(Total, Index)                        \
  ((((Index) + 1) * MAX_BLOCK_SIZE) <= (Total)          \
   ? MAX_BLOCK_SIZE                                     \
   : (Total) % MAX_BLOCK_SIZE)
