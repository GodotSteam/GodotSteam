extends Node

@export_file("*.xml") var xml
@export var sprite_sheets:Texture
@export_dir var output


func _ready():
	var parser = XMLParser.new()
	parser.open(xml)
	while parser.read() == OK:
		var file = {}
		file.name = parser.get_node_name()
#		file.data = parser.get_node_data()
		file.type = parser.get_node_type()
		for i in parser.get_attribue_count():
			file[parser.get_attribute_name(i)] = parser.get_attribute_value(i)
		if file.name.ends_with("png"):
			var asset := AtlasTexture.new() as AtlasTexture
			asset.atlas = sprite_sheets
			var title = file.name.left(file.name.length()-4)
			var pos = Vector2(file.x as float,file.y as float)
			var size = Vector2(file.width as float,file.height as float)
			asset.region = Rect2(pos,size)
			asset.filter_clip = true
			var qwer = output + "/" + title + ".tres"
			ResourceSaver.save(asset,qwer)
			pass
		pass
