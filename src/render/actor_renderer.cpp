#include "i_render.h"

void ActorRenderer::Init()
{
	mVAO.Init();
}

ActorRenderer::ActorRenderer()
{
	Init();
}

void ActorRenderer::Draw( Scene const& Object )
{
	ActorList_t const& Lst=Object.GetActors();
	if(Lst.empty())return;
	static size_t PrevVecSize=0;
	typedef std::vector<RenderableSprite> RenderableSprites_t;
	RenderableSprites_t RenderableSprites;
	RenderableSprites.reserve(PrevVecSize);
	for(ActorList_t::const_iterator i=Lst.begin(),e=Lst.end();i!=e;++i)
	{
		const Actor& Object=*i;
		Actor::ActionList_t const& Actions=Object.GetActions();
		static RenderableRepo& Rend(RenderableRepo::Get());
		SpriteCollection const& Sprites=Rend(Object.GetId());
		for(Actor::ActionList_t::const_iterator i=Actions.begin(),e=Actions.end();i!=e;++i)
		{
			Action const& Act=*i->second;
			int32_t const ActId=i->first;
			Sprite const& Spr=Sprites(ActId);
			if(!Spr.IsValid()) continue;
			SpritePhase const& Phase=Spr((int32_t)Act.GetState());
			//for(size_t test=0;test<100;++test)
			RenderableSprites.push_back(RenderableSprite(&Object,ActId,&Spr,&Phase));
		}
		Actor::ItemList_t const& items=Object.GetItems();
		for(Actor::ItemList_t::const_iterator i=items.begin(),e=items.end();i!=e;++i)
		{
			Item const& Act=*i;
			int32_t const ActId=Act.GetId();
			Sprite const& Spr=Sprites(ActId);
			if(!Spr.IsValid()) continue;
			SpritePhase const& Phase=Spr((int32_t)Act.GetState());
			//for(size_t test=0;test<100;++test)
			RenderableSprites.push_back(RenderableSprite(&Object,ActId,&Spr,&Phase));
		}
	}

	// TODO: sort Z order, alive state es texture id alapjan.
	// Meg persze (last cmp) pointerek szerint, hogy determinisztikus legyen.
	std::sort(RenderableSprites.begin(),RenderableSprites.end(),RenderableSpriteCompare());

	size_t CurSize=RenderableSprites.size();
	typedef std::vector<glm::vec2> Positions_t;
	Positions_t Positions;
	Positions.reserve(CurSize);
	typedef std::vector<GLfloat> Floats_t;
	Floats_t Headings;
	Headings.reserve(CurSize);
	Floats_t Sizes;
	Sizes.reserve(CurSize);
	typedef std::vector<glm::vec4> TexCoords_t;
	TexCoords_t TexCoords;
	TexCoords.reserve(CurSize);

	struct CountByTexId
	{
		GLuint TexId;
		size_t Start;
		size_t Count;
		CountByTexId(GLuint t,size_t s,size_t c):TexId(t),Start(s),Count(c){}
	};
	typedef std::vector<CountByTexId> Counts_t;
	Counts_t Counts;
	GLuint TexId=-1;
	size_t Count=0;
	size_t Start=0;
	size_t MaxCount=0;
	for(RenderableSprites_t::const_iterator i=RenderableSprites.begin(),e=RenderableSprites.end();i!=e;++i)
	{
		if(TexId!=i->Spr->TexId)
		{
			if(Count)
				Counts.push_back(CountByTexId(TexId,Start,Count));
			Start+=Count;
			Count=0;
			TexId=i->Spr->TexId;
		}
		++Count;
		if(Count>MaxCount)
			MaxCount=Count;
		Positions.push_back(glm::vec2(i->Obj->GetX(),i->Obj->GetY()));
		Headings.push_back((GLfloat)i->Obj->GetOrientation());
		Sizes.push_back((GLfloat)(i->Obj->GetRadius()*i->Anim->GetScale()));
		TexCoords.push_back(glm::vec4(i->Spr->Left,i->Spr->Right,i->Spr->Bottom,i->Spr->Top));
	}
	if(Count)
		Counts.push_back(CountByTexId(TexId,Start,Count));

	if(CurSize==0)
		return;

	mVAO.Bind();
	if(CurSize!=PrevVecSize)
	{
		PrevVecSize=CurSize;
		size_t TotalSize=CurSize*(sizeof(glm::vec4)+sizeof(glm::vec2)+2*sizeof(GLfloat));
		glBufferData(GL_ARRAY_BUFFER, TotalSize, NULL, GL_DYNAMIC_DRAW);
	}

	size_t CurrentOffset=0;
	size_t CurrentSize=CurSize*sizeof(glm::vec4);
	GLuint CurrentAttribIndex=0;
	glBufferSubData(GL_ARRAY_BUFFER,CurrentOffset,CurrentSize,&TexCoords[0]);
	glEnableVertexAttribArray(CurrentAttribIndex);
	size_t const TexIndex=CurrentOffset;
	++CurrentAttribIndex;

	CurrentOffset+=CurrentSize;
	CurrentSize=CurSize*sizeof(glm::vec2);
	glBufferSubData(GL_ARRAY_BUFFER,CurrentOffset,CurrentSize,&Positions[0]);
	glEnableVertexAttribArray(CurrentAttribIndex);
	size_t const PosIndex=CurrentOffset;
	++CurrentAttribIndex;

	CurrentOffset+=CurrentSize;
	CurrentSize=CurSize*sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER,CurrentOffset,CurrentSize,&Headings[0]);
	glEnableVertexAttribArray(CurrentAttribIndex);
	size_t const HeadingIndex=CurrentOffset;
	++CurrentAttribIndex;

	CurrentOffset+=CurrentSize;
	CurrentSize=CurSize*sizeof(GLfloat);
	glBufferSubData(GL_ARRAY_BUFFER,CurrentOffset,CurrentSize,&Sizes[0]);
	glEnableVertexAttribArray(CurrentAttribIndex);
	size_t const SizeIndex=CurrentOffset;

	ShaderManager& ShaderMgr(ShaderManager::Get());
	ShaderMgr.ActivateShader("sprite2");
	ShaderMgr.UploadData("spriteTexture",GLuint(1));
	glActiveTexture(GL_TEXTURE0+1);
	for(Counts_t::const_iterator i=Counts.begin(),e=Counts.end();i!=e;++i)
	{
		CountByTexId const& Part=*i;
		CurrentAttribIndex=0;
		glVertexAttribPointer(CurrentAttribIndex, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(TexIndex+sizeof(glm::vec4)*Part.Start));
		glVertexAttribDivisor(CurrentAttribIndex, 1);
		++CurrentAttribIndex;
		glVertexAttribPointer(CurrentAttribIndex, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(PosIndex+sizeof(glm::vec2)*Part.Start));
		glVertexAttribDivisor(CurrentAttribIndex, 1);
		++CurrentAttribIndex;
		glVertexAttribPointer(CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(HeadingIndex+sizeof(GLfloat)*Part.Start));
		glVertexAttribDivisor(CurrentAttribIndex, 1);
		++CurrentAttribIndex;
		glVertexAttribPointer(CurrentAttribIndex, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(SizeIndex+sizeof(GLfloat)*Part.Start));
		glVertexAttribDivisor(CurrentAttribIndex, 1);
		glBindTexture(GL_TEXTURE_2D,Part.TexId);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP,0,4,Part.Count);
	}
	glActiveTexture(GL_TEXTURE0);
	mVAO.Unbind();
}

ActorRenderer::~ActorRenderer()
{

}

bool ActorRenderer::RenderableSpriteCompare::operator()( RenderableSprite const& Rs1, RenderableSprite const& Rs2 )
{
	return	(int)Rs1.Obj->IsAlive() < (int)Rs2.Obj->IsAlive() ||
		Rs1.Obj->IsAlive() == Rs2.Obj->IsAlive() &&
		((Rs1.Obj->IsAlive() &&
		(Rs1.Obj->GetGUID() < Rs2.Obj->GetGUID() ||
		Rs1.Obj->GetGUID() == Rs2.Obj->GetGUID() &&
		(Rs1.ActId < Rs2.ActId ||
		Rs1.ActId==Rs2.ActId &&
		Rs1.Spr->TexId < Rs2.Spr->TexId)))
		||
		(!Rs1.Obj->IsAlive() &&
		(Rs1.Obj->GetTimeOfDeath() < Rs2.Obj->GetTimeOfDeath() ||
		Rs1.Obj->GetTimeOfDeath() == Rs2.Obj->GetTimeOfDeath() &&
		(Rs1.ActId < Rs2.ActId ||
		Rs1.ActId==Rs2.ActId &&
		Rs1.Spr->TexId < Rs2.Spr->TexId))));
}