from drawtree import draw_bst

with open("a.tree") as f:
    nums = [int(x) for x in f.read().split()]
    draw_bst(nums)