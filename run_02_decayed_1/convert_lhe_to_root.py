import xml.etree.ElementTree as ET
import ROOT

# 打开 LHE 文件
lhe_file = "unweighted_events.lhe"
tree = ET.parse(lhe_file)
root = tree.getroot()

# 创建 ROOT 文件和 TTree
output_file = ROOT.TFile("events.root", "RECREATE")
tree = ROOT.TTree("event_tree", "LHE Events")

# 定义变量
px = ROOT.std.vector('float')()
py = ROOT.std.vector('float')()
pz = ROOT.std.vector('float')()
E = ROOT.std.vector('float')()
pid = ROOT.std.vector('int')()

tree.Branch("px", px)
tree.Branch("py", py)
tree.Branch("pz", pz)
tree.Branch("E", E)
tree.Branch("pid", pid)

# 遍历 LHE 文件中的事件
for event in root.findall('event'):
    lines = event.text.strip().split("\n")[3:]  # 跳过第一行（头信息）
    px.clear(); py.clear(); pz.clear(); E.clear(); pid.clear()
    for line in lines:
        data = line.split()
        px.push_back(float(data[6]))
        py.push_back(float(data[7]))
        pz.push_back(float(data[8]))
        E.push_back(float(data[9]))
        pid.push_back(int(data[0]))
    tree.Fill()

# 保存 ROOT 文件
output_file.Write()
output_file.Close()
